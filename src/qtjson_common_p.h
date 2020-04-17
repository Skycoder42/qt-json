#ifndef QTJSON_COMMON_H
#define QTJSON_COMMON_H

#include <optional>

#include <QtCore/QMetaClassInfo>
#include <QtCore/QVariant>
#include <QtCore/QJsonValue>
#include <QtCore/QCborValue>

namespace QtJson::__private {

template <typename TType>
struct DataValueInfo;

template <>
struct DataValueInfo<QJsonValue> {
    using Value = QJsonValue;
    using Map = QJsonObject;
    using List = QJsonArray;

    static constexpr auto Undefined = QJsonValue::Undefined;
    static constexpr auto Null = QJsonValue::Null;
};

template <>
struct DataValueInfo<QCborValue> {
    using Value = QCborValue;
    using Map = QCborMap;
    using List = QCborArray;

    static constexpr auto Undefined = QCborValue::Undefined;
    static constexpr auto Null = QCborValue::Null;
};

QTJSON_EXPORT QVariant findInfo(const QMetaObject *metaObject, const char *key, const QVariant &defaultValue = {});

template <typename T>
std::optional<T> findInfo(const QMetaObject *metaObject, const char *key) {
    const auto info = findInfo(metaObject, key);
    if (info.isValid())
        return info.template value<T>();
    else
        return std::nullopt;
}

template <typename T>
T findInfo(const QMetaObject *metaObject, const char *key, const T &defaultValue) {
    return findInfo(metaObject, key, QVariant::fromValue(defaultValue)).template value<T>();
}

}

#endif // QTJSON_COMMON_H
