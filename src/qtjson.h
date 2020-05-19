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

namespace QtJson::__private {

constexpr const char * const CborSignature = "\xd9\xd9\xf7";

}

namespace QtJson {

QTJSON_EXPORT QString writeJson(const QJsonValue &value, QJsonDocument::JsonFormat format = QJsonDocument::Indented);
QTJSON_EXPORT QJsonValue readJson(const QString &data);
QTJSON_EXPORT QByteArray writeCbor(const QCborValue &value, QCborValue::EncodingOptions opt = QCborValue::NoTransformation);
QTJSON_EXPORT QCborValue readCbor(const QByteArray &data, bool requireSignature = false);

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
inline QString stringify(const TValue &value,
                         const Configuration &config = {},
                         QJsonDocument::JsonFormat format = QJsonDocument::Indented) {
    return writeJson(toJson(value, config), format);
}

template <typename TValue>
inline QByteArray binarify(const TValue &value,
                    const Configuration &config = {},
                    QCborValue::EncodingOptions opt = QCborValue::NoTransformation) {
    return writeCbor(toCbor(value, config), opt);
}

template <typename TValue>
inline TValue parseString(const QString &jsonString, const Configuration &config = {}) {
    return fromJson<TValue>(readJson(jsonString), config);
}

template <typename TValue>
inline TValue parseBinary(const QByteArray &cborData, const Configuration &config = {}, bool requireSignature = false) {
    return fromCbor<TValue>(readCbor(cborData, requireSignature), config);
}

template <typename TValue>
inline TValue parseAny(const QByteArray &data, const Configuration &config = {}) {
    if (data.mid(0, 3) == __private::CborSignature)
        return parseBinary(data, config, false);
    else
        return parseString(QString::fromUtf8(data), config);
}

}
