#include "serializablegadget.h"
#include "qtjson_common_p.h"
#include "qtjson_exception.h"
#include "serializationadapter.h"
#include <QtCore/QMetaProperty>
#include <QtCore/QMetaMethod>
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

        typename DataValueInfo<TValue>::Value value {DataValueInfo<TValue>::Undefined};
        auto variant = property.readOnGadget(gadget);
        if (!variant.isValid())
            continue;

        if (const auto serializable = SerializationAdapter::obtainSerializable(gadget, metaObject, property, variant);
			serializable) {
			if constexpr (std::is_same_v<TValue, QCborValue>)
				value = serializable->toCbor(config);
			else
				value = serializable->toJson(config);
		} else if (property.isEnumType()) {
			if (config.enumAsString) {
				const auto metaEnum = property.enumerator();
				if (metaEnum.isFlag())
					value = QString::fromUtf8(metaEnum.valueToKeys(variant.toInt()));
				else
					value = QString::fromUtf8(metaEnum.valueToKey(variant.toInt()));
			} else
				value = variant.toInt();
		} else
			value = TValue::fromVariant(variant);

		map.insert(QString::fromUtf8(property.name()), value);
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
		QVariant variant{property.userType(), nullptr};

        if (const auto serializable = SerializationAdapter::obtainSerializable(gadget, metaObject, property, variant);
			serializable) {
			if constexpr (std::is_same_v<TValue, QCborValue>)
				serializable->assignCbor(value, config);
			else
				serializable->assignJson(value, config);
		} else if (property.isEnumType()) {
			if (config.enumAsString) {
				const auto key = value.toString().toUtf8();
				const auto metaEnum = property.enumerator();
				if (metaEnum.isFlag())
					variant = metaEnum.keysToValue(key.constData());
				else
					variant = metaEnum.keyToValue(key.constData());
			} else {
				if constexpr(std::is_same_v<TValue, QCborValue>)
					variant = static_cast<int>(value.toInteger());
				else
					variant = value.toInt();
			}
		} else
			variant = value.toVariant();

		if (!property.writeOnGadget(gadget, variant))
			throw InvalidPropertyValueException{property, value};
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
