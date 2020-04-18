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
typename DataValueInfo<TValue>::Map serialize(const QMetaObject *metaObject, const QtJson::SerializableGadget *gadget, const typename DataValueInfo<TValue>::Config &config)
{
    const auto offset = findInfo<int>(metaObject, QTJSON_PROPERTY_OFFSET_KEY, 0);
    typename DataValueInfo<TValue>::Map map;
    for (auto i = offset; i < metaObject->propertyCount(); ++i) {
        const auto property = metaObject->property(i);
        if (!property.isStored())
            continue;
        const auto value = property.readOnGadget(gadget);

        if (property.isEnumType()) {
            if (config.enumAsString) {
                const auto metaEnum = property.enumerator();
                if (metaEnum.isFlag()) {
                    map.insert(QString::fromUtf8(property.name()),
                               QString::fromUtf8(metaEnum.valueToKeys(value.toInt())));
                } else {
                    map.insert(QString::fromUtf8(property.name()),
                               QString::fromUtf8(metaEnum.valueToKey(value.toInt())));
                }
            } else
                map.insert(QString::fromUtf8(property.name()), value.toInt());
        } else {
            // TODO check for: ISerializable
            map.insert(QString::fromUtf8(property.name()), TValue::fromVariant(value));
        }
    }
    return map;
}

template <typename TValue>
void deserialize(const QMetaObject *metaObject, QtJson::SerializableGadget *gadget, const typename DataValueInfo<TValue>::Map &map, const typename DataValueInfo<TValue>::Config &config)
{
    const auto offset = findInfo<int>(metaObject, QTJSON_PROPERTY_OFFSET_KEY, 0);
    // TODO check for strict rules
    for (auto i = offset; i < metaObject->propertyCount(); ++i) {
        const auto property = metaObject->property(i);
        if (!property.isStored())
            continue;

        const auto value = map.value(QString::fromUtf8(property.name()));
        if (property.isEnumType()) {
            if (config.enumAsString) {
                const auto key = value.toString().toUtf8();
                const auto metaEnum = property.enumerator();
                int intVal = 0;
                if (metaEnum.isFlag())
                    intVal = metaEnum.keysToValue(key.constData());
                else
                    intVal = metaEnum.keyToValue(key.constData());
                if (!property.writeOnGadget(gadget, intVal))
                    throw InvalidPropertyValueException{property, value};
            } else {
                if constexpr(std::is_same_v<TValue, QCborValue>) {
                    if (!property.writeOnGadget(gadget, static_cast<int>(value.toInteger())))
                        throw InvalidPropertyValueException{property, value};
                } else {
                    if (!property.writeOnGadget(gadget, value.toInt()))
                        throw InvalidPropertyValueException{property, value};
                }

            }
        } else {
            // TODO check for: ISerializable
            const auto value = map.value(QString::fromUtf8(property.name())).toVariant();
            if (!property.writeOnGadget(gadget, value))
                throw InvalidPropertyValueException{property, value};
        }
    }
}

}

QJsonValue SerializableGadget::toJson(const JsonConfiguration &config) const
{
    return serialize<QJsonValue>(getMetaObject(), this, config);
}

void SerializableGadget::assignJson(const QJsonValue &value, const JsonConfiguration &config)
{
    deserialize<QJsonValue>(getMetaObject(), this, value.toObject(), config);
}

QCborValue SerializableGadget::toCbor(const CborConfiguration &config) const
{
    return serialize<QCborValue>(getMetaObject(), this, config);
}

void SerializableGadget::assignCbor(const QCborValue &value, const CborConfiguration &config)
{
    deserialize<QCborValue>(getMetaObject(), this, value.toMap(), config);
}
