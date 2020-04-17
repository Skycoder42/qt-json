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

        if (property.isEnumType()) {
            const auto metaEnum = property.enumerator();
            if (metaEnum.isFlag()) {
                map.insert(QString::fromUtf8(property.name()),
                           QString::fromUtf8(metaEnum.valueToKeys(value.toInt())));
            } else {
                map.insert(QString::fromUtf8(property.name()),
                           QString::fromUtf8(metaEnum.valueToKey(value.toInt())));
            }
        } else {
            // TODO check for: ISerializable
            map.insert(QString::fromUtf8(property.name()), TValue::fromVariant(value));
        }
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

        if (property.isEnumType()) {
            const auto key = map.value(QString::fromUtf8(property.name())).toString().toUtf8();
            const auto metaEnum = property.enumerator();
            int value = 0;
            if (metaEnum.isFlag())
                value = metaEnum.keysToValue(key.constData());
            else
                value = metaEnum.keyToValue(key.constData());
            if (!property.writeOnGadget(gadget, value))
                throw InvalidPropertyValueException{property, QString::fromUtf8(key)};
        } else {
            // TODO check for: ISerializable
            const auto value = map.value(QString::fromUtf8(property.name())).toVariant();
            if (!property.writeOnGadget(gadget, value))
                throw InvalidPropertyValueException{property, value};
        }
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
