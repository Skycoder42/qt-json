#include "serializablegadget.h"
#include "qtjson_common_p.h"
#include <QtCore/QMetaProperty>
#include <QtCore/QJsonObject>
#include <QtCore/QCborMap>
using namespace QtJson;

QJsonValue SerializableGadget::toJson() const
{
    const auto mo = getMetaObject();
    const auto offset = findInfo<int>(mo, QTJSON_PROPERTY_OFFSET_KEY, 0);
    QJsonObject jObj;
    for (auto i = offset; i < mo->propertyCount(); ++i) {
        const auto property = mo->property(i);
        if (!property.isStored())
            continue;
        const auto value = property.readOnGadget(this);
        // TODO check for: enum, ISerializable
        jObj.insert(QString::fromUtf8(property.name()), QJsonValue::fromVariant(value));
    }
    return jObj;
}

void SerializableGadget::assignJson(const QJsonValue &value)
{
    const auto jObj = value.toObject();
    const auto mo = getMetaObject();
    const auto offset = findInfo<int>(mo, QTJSON_PROPERTY_OFFSET_KEY, 0);
    // TODO check for strict rules
    for (auto i = offset; i < mo->propertyCount(); ++i) {
        const auto property = mo->property(i);
        if (!property.isStored())
            continue;
        // TODO check for: enum, ISerializable
        const auto value = jObj.value(QString::fromUtf8(property.name())).toVariant();
        if (!property.writeOnGadget(this, value)) {
            // TODO throw
        }
    }
}

QCborValue SerializableGadget::toCbor() const
{
    const auto mo = getMetaObject();
    const auto offset = findInfo<int>(mo, QTJSON_PROPERTY_OFFSET_KEY, 0);
    QCborMap cMap;
    for (auto i = offset; i < mo->propertyCount(); ++i) {
        const auto property = mo->property(i);
        if (!property.isStored())
            continue;
        const auto value = property.readOnGadget(this);
        // TODO check for: enum, ISerializable
        cMap.insert(QString::fromUtf8(property.name()), QCborValue::fromVariant(value));
    }
    return cMap;
}

void SerializableGadget::assignCbor(const QCborValue &value)
{
    const auto cMap = value.toMap();
    const auto mo = getMetaObject();
    const auto offset = findInfo<int>(mo, QTJSON_PROPERTY_OFFSET_KEY, 0);
    // TODO check for strict rules
    for (auto i = offset; i < mo->propertyCount(); ++i) {
        const auto property = mo->property(i);
        if (!property.isStored())
            continue;
        // TODO check for: enum, ISerializable
        const auto value = cMap.value(QString::fromUtf8(property.name())).toVariant();
        if (!property.writeOnGadget(this, value)) {
            // TODO throw
        }
    }
}
