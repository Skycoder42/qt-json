#include "serializablegadget.h"
#include "qtjson_common_p.h"
#include "qtjson_exception.h"
#include <QtCore/QMetaProperty>
#include <QtCore/QJsonObject>
#include <QtCore/QCborMap>
using namespace QtJson;
using namespace QtJson::__private;

namespace {

template <typename TValue>
typename DataValueInfo<TValue>::Map serialize(const QMetaObject *metaObject, const QtJson::SerializableGadget *gadget)
{
    const auto offset = findInfo<int>(metaObject, QTJSON_PROPERTY_OFFSET_KEY, 0);
    typename DataValueInfo<TValue>::Map map;
    for (auto i = offset; i < metaObject->propertyCount(); ++i) {
        const auto property = metaObject->property(i);
        if (!property.isStored())
            continue;
        const auto value = property.readOnGadget(gadget);
        // TODO check for: enum, ISerializable
        map.insert(QString::fromUtf8(property.name()), TValue::fromVariant(value));
    }
    return map;
}

template <typename TValue>
void deserialize(const QMetaObject *metaObject, QtJson::SerializableGadget *gadget, const typename DataValueInfo<TValue>::Map &map)
{
    const auto offset = findInfo<int>(metaObject, QTJSON_PROPERTY_OFFSET_KEY, 0);
    // TODO check for strict rules
    for (auto i = offset; i < metaObject->propertyCount(); ++i) {
        const auto property = metaObject->property(i);
        if (!property.isStored())
            continue;
        // TODO check for: enum, ISerializable
        const auto var = map.value(QString::fromUtf8(property.name())).toVariant();
        if (!property.writeOnGadget(gadget, var))
            throw InvalidPropertyValueException{property, var};
    }
}

}

QJsonValue SerializableGadget::toJson() const
{
    return serialize<QJsonValue>(getMetaObject(), this);
}

void SerializableGadget::assignJson(const QJsonValue &value)
{
    deserialize<QJsonValue>(getMetaObject(), this, value.toObject());
}

QCborValue SerializableGadget::toCbor() const
{
    return serialize<QCborValue>(getMetaObject(), this);
}

void SerializableGadget::assignCbor(const QCborValue &value)
{
    deserialize<QCborValue>(getMetaObject(), this, value.toMap());
}
