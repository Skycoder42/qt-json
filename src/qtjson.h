#pragma once

#include "configuration.h"

#include <QtCore/QJsonValue>
#include <QtCore/QCborValue>
#include <QtCore/QVariant>

namespace QtJson
{

QTJSON_EXPORT QJsonValue stringify(const QVariant &value, const CommonConfiguration &configuration = {});
template <typename TValue>
inline QJsonValue stringify(const TValue &value, const CommonConfiguration &configuration = {}) {
    return stringify(QVariant::fromValue(value), configuration);
}

QTJSON_EXPORT QCborValue binarify(const QVariant &value, const CommonConfiguration &configuration = {});
template <typename TValue>
inline QCborValue binarify(const TValue &value, const CommonConfiguration &configuration = {}) {
    return binarify(QVariant::fromValue(value), configuration);
}

}
