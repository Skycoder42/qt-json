#pragma once

#include "configuration.h"
#include "iserializable.h"
#include "qtjson_exception.h"

#include "serializablegadget.h"

#include "serializableadapter.h"
#include "serializablebytearray.h"
#include "serializabledatetime.h"
#include "serializableversionnumber.h"
#include "serializableoptional.h"
#include "serializablearray.h"
#include "serializabledictionary.h"

#include <QtCore/QJsonDocument>

namespace QtJson {

template <typename TValue>
inline QJsonValue toJson(const TValue &value, const Configuration &config = {}) {
	return SerializableAdapter<TValue>::toJson(value, config);
}

template <typename TValue>
inline TValue fromJson(const QJsonValue &data, const Configuration &config = {}) {
	return SerializableAdapter<TValue>::fromJson(data, config);
}

template <typename TValue>
inline QCborValue toCbor(const TValue &value, const Configuration &config = {}) {
	return SerializableAdapter<TValue>::toCbor(value, config);
}

template <typename TValue>
inline TValue fromCbor(const QCborValue &data, const Configuration &config = {}) {
	return SerializableAdapter<TValue>::fromCbor(data, config);
}

template <typename TValue>
QString stringify(const TValue &value,
				  const Configuration &config = {},
				  QJsonDocument::JsonFormat format = QJsonDocument::Indented) {
	const auto json = toJson(value, config);
	switch (json.type()) {
	case QJsonValue::Undefined:
		throw InvalidDataException{};
	case QJsonValue::Null:
	case QJsonValue::Bool:
	case QJsonValue::Double:
	case QJsonValue::String: {
		const auto data = QJsonDocument{QJsonArray{json}}.toJson(QJsonDocument::Compact);
		return QString::fromUtf8(data.mid(1, data.size() - 2));
	}
	case QJsonValue::Array:
		return QString::fromUtf8(QJsonDocument{json.toArray()}.toJson(format));
	case QJsonValue::Object:
		return QString::fromUtf8(QJsonDocument{json.toObject()}.toJson(format));
	default:
		Q_UNREACHABLE();
	}
}

template <typename TValue>
QByteArray binarify(const TValue &value,
					const Configuration &config = {},
					QCborValue::EncodingOptions opt = QCborValue::NoTransformation) {
	const auto cbor = toCbor(value, config);
	if (cbor.isInvalid())
		throw InvalidDataException{};
	return QCborValue{QCborKnownTags::Signature, cbor}.toCbor(opt);
}

template <typename TValue>
inline TValue parseString(const QString &jsonString, const Configuration &config = {}) {
	QJsonParseError error;
	QJsonValue value{QJsonValue::Undefined};
	const auto doc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);
	if (error.error == QJsonParseError::IllegalValue) {
		QJsonParseError subError;
		const auto subDoc = QJsonDocument::fromJson("[" + jsonString.toUtf8() + "]", &subError);
		if (error.error != QJsonParseError::NoError)
			throw JsonParseException{error};
		else
			value = subDoc.array().first();
	} else if (error.error != QJsonParseError::NoError)
		throw JsonParseException{error};
	else {
		if (doc.isObject())
			value = doc.object();
		else if (doc.isArray())
			value = doc.array();
		else if (doc.isNull())
			value = QJsonValue::Null;
		else
			throw InvalidDataException{};
	}

	return fromJson<TValue>(value, config);
}

template <typename TValue>
inline TValue parseBinary(const QByteArray &cborData, const Configuration &config = {}, bool requireSignature = false) {
	if (requireSignature &&
		cborData.mid(0, 3) != "\xd9\xd9\xf7") {
		throw CborParseException{cborData.mid(0, 3)};
	}

	QCborParserError error;
	const QCborValue cbor = QCborValue::fromCbor(cborData, &error);
	if (error.error != QCborError::NoError)
		throw CborParseException{error};
	else {
		if (cbor.isTag() && cbor.tag() == QCborKnownTags::Signature)
			return fromCbor<TValue>(cbor.taggedValue(), config);
		else
			return fromCbor<TValue>(cbor, config);
	}
}

template <typename TValue>
inline TValue parseAny(const QByteArray &data, const Configuration &config = {}) {
	if (data.mid(0, 3) == "\xd9\xd9\xf7")
		return parseBinary<TValue>(data, config, false);
	else
		return parseString<TValue>(QString::fromUtf8(data), config);
}

}
