#include "serializablebytearray.h"
#include "qtjson_exception.h"
#include "qtjson_helpers.h"
using namespace QtJson;

QJsonValue QtJson::SerializableAdapter<QByteArray, void>::toJson(const QByteArray &value, const Configuration &config)
{
	switch (config.byteArrayMode) {
	case ByteArrayMode::Base64:
		return QString::fromUtf8(value.toBase64(QByteArray::Base64Encoding | QByteArray::KeepTrailingEquals));
	case ByteArrayMode::Base64url:
		return QString::fromUtf8(value.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
	case ByteArrayMode::Hex:
		return QString::fromUtf8(value.toHex());
	default:
		Q_UNREACHABLE();
	}
}

QByteArray QtJson::SerializableAdapter<QByteArray, void>::fromJson(const QJsonValue &value, const Configuration &config)
{
	if (!value.isString()) {
		throw InvalidValueTypeException{value.type(), {QJsonValue::String}};
	}
	switch (config.byteArrayMode) {
	case ByteArrayMode::Base64:
		return QByteArray::fromBase64(value.toString().toUtf8(), QByteArray::Base64Encoding);
	case ByteArrayMode::Base64url:
		return QByteArray::fromBase64(value.toString().toUtf8(), QByteArray::Base64UrlEncoding);
	case ByteArrayMode::Hex:
		return QByteArray::fromHex(value.toString().toUtf8());
	default:
		Q_UNREACHABLE();
	}
}

QCborValue QtJson::SerializableAdapter<QByteArray, void>::toCbor(const QByteArray &value, const Configuration &config)
{
	switch (config.byteArrayMode) {
	case ByteArrayMode::Base64:
		return {QCborKnownTags::ExpectedBase64, QCborValue{value}};
	case ByteArrayMode::Base64url:
		return {QCborKnownTags::ExpectedBase64url, QCborValue{value}};
	case ByteArrayMode::Hex:
		return {QCborKnownTags::ExpectedBase16, QCborValue{value}};
	default:
		Q_UNREACHABLE();
	}
}

QByteArray QtJson::SerializableAdapter<QByteArray, void>::fromCbor(const QCborValue &value, const Configuration &config)
{
	Q_UNUSED(config);
	const auto xValue = __private::extract(value);
	if (!xValue.isByteArray())
		throw InvalidValueTypeException{xValue.type(), {QCborValue::ByteArray}};
	return (value.isTag() ? value.taggedValue() : value).toByteArray();
}
