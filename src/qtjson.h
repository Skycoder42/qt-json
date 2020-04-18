#pragma once

#include "configuration.h"
#include "iserializablewrapperfactory.h"

#include <QtCore/QJsonValue>
#include <QtCore/QCborValue>
#include <QtCore/QVariant>

namespace QtJson
{

QTJSON_EXPORT QJsonValue stringify(const QVariant &value, const JsonConfiguration &configuration = {});
template <typename TValue>
inline QJsonValue stringify(const TValue &value, const JsonConfiguration &configuration = {}) {
    return stringify(QVariant::fromValue(value), configuration);
}

QTJSON_EXPORT QCborValue binarify(const QVariant &value, const CborConfiguration &configuration = {});
template <typename TValue>
inline QCborValue binarify(const TValue &value, const CborConfiguration &configuration = {}) {
    return binarify(QVariant::fromValue(value), configuration);
}

QTJSON_EXPORT void registerWrapperFactory(int typeId, ISerializableWrapperFactory *factory);
template <typename TType>
void registerWrapperFactory(ITypedSerializableWrapperFactory<TType> * factory) {
    registerWrapperFactory(qMetaTypeId<TType>(), factory);
}

}
