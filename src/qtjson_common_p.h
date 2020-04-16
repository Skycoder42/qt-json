#ifndef QTJSON_COMMON_H
#define QTJSON_COMMON_H

#include "qtjson_global.h"

#include <optional>

#include <QtCore/QMetaClassInfo>
#include <QtCore/QVariant>

namespace QtJson {

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
