#include "serializablegadget.h"
#include "serializablegadget_p.h"
#include "qtjson_helpers.h"
#include "qtjson_exception.h"
#include <QtCore/QMetaProperty>
#include <QtCore/QMetaMethod>
#include <QtCore/QJsonObject>
#include <QtCore/QCborMap>
#include <QtCore/QSet>
using namespace QtJson;
using namespace QtJson::__private;

QJsonValue SerializableGadget::toJson(const Configuration &config) const
{
	return serialize<QJsonValue>(config);
}

void SerializableGadget::assignJson(const QJsonValue &value, const Configuration &config)
{
	if (!value.isObject())
		throw InvalidValueTypeException{value.type(), {QJsonValue::Object}};
	deserialize<QJsonValue>(value.toObject(), config);
}

QCborValue SerializableGadget::toCbor(const Configuration &config) const
{
	return serialize<QCborValue>(config);
}

void SerializableGadget::assignCbor(const QCborValue &value, const Configuration &config)
{
	if (!value.isMap())
		throw InvalidValueTypeException{value.type(), {QCborValue::Map}};
	deserialize<QCborValue>(value.toMap(), config);
}

std::optional<QMetaMethod> SerializableGadget::findMethod(const QMetaObject *metaObject, const QMetaProperty &property, AdapterMethod method) const
{
	const QByteArray propName = property.name();
	QByteArray rawMethodSignature;
	switch (method) {
	case AdapterMethod::ToJson:
		rawMethodSignature = QTJSON_SERIALIZABLE_PROP_TO_JSON_SIGNATURE(propName);
		break;
	case AdapterMethod::FromJson:
		rawMethodSignature = QTJSON_SERIALIZABLE_PROP_FROM_JSON_SIGNATURE(propName);
		break;
	case AdapterMethod::ToCbor:
		rawMethodSignature = QTJSON_SERIALIZABLE_PROP_TO_CBOR_SIGNATURE(propName);
		break;
	case AdapterMethod::FromCbor:
		rawMethodSignature = QTJSON_SERIALIZABLE_PROP_FROM_CBOR_SIGNATURE(propName);
		break;
	default:
		Q_UNREACHABLE();
	}

    const auto mIdx = metaObject->indexOfMethod(QMetaObject::normalizedSignature(rawMethodSignature));
    if (mIdx < 0)
		return std::nullopt;
	else
        return metaObject->method(mIdx);
}

QJsonValue SerializableGadget::propToJson(const QMetaObject *metaObject, const QMetaProperty &property, const Configuration &config) const
{
    const auto method = findMethod(metaObject, property, AdapterMethod::ToJson);
    if (!method)
        return QJsonValue::Undefined;

    QJsonValue result;
    if (!method->invokeOnGadget(const_cast<SerializableGadget*>(this),
                                Q_RETURN_ARG(QJsonValue, result),
                                Q_ARG(QtJson::Configuration, config))) {
        throw InvalidPropertyMethodCallException(property, *method);
    }
    return result;
}

bool SerializableGadget::propAssignJson(const QMetaObject *metaObject, const QMetaProperty &property, const QJsonValue &value, const Configuration &config)
{
    const auto method = findMethod(metaObject, property, AdapterMethod::FromJson);
    if (!method)
        return false;

    if (!method->invokeOnGadget(this,
                                Q_ARG(QJsonValue, value),
                                Q_ARG(QtJson::Configuration, config))) {
        throw InvalidPropertyMethodCallException(property, *method);
    }
    return true;
}

QCborValue SerializableGadget::propToCbor(const QMetaObject *metaObject, const QMetaProperty &property, const Configuration &config) const
{
    const auto method = findMethod(metaObject, property, AdapterMethod::ToCbor);
    if (!method)
        return QCborValue::Invalid;

    QCborValue result;
    if (!method->invokeOnGadget(const_cast<SerializableGadget*>(this),
                                Q_RETURN_ARG(QCborValue, result),
                                Q_ARG(QtJson::Configuration, config))) {
        throw InvalidPropertyMethodCallException(property, *method);
    }
    return result;
}

bool SerializableGadget::propAssignCbor(const QMetaObject *metaObject, const QMetaProperty &property, const QCborValue &value, const Configuration &config)
{
    const auto method = findMethod(metaObject, property, AdapterMethod::FromCbor);
    if (!method)
        return false;

    if (!method->invokeOnGadget(this,
                                Q_ARG(QCborValue, value),
                                Q_ARG(QtJson::Configuration, config))) {
        throw InvalidPropertyMethodCallException(property, *method);
    }
    return true;
}

template<typename TValue>
TValue SerializableGadget::findInfo(const QMetaObject *metaObject, const char *key, const TValue &defaultValue) const
{
	const auto cIdx = metaObject->indexOfClassInfo(key);
	if (cIdx < 0)
		return defaultValue;
	else
		return QVariant{QString::fromUtf8(metaObject->classInfo(cIdx).value())}.template value<TValue>();
}

template<typename TValue>
typename DataValueInfo<TValue>::Map SerializableGadget::serialize(const Configuration &config) const
{
    const auto metaObject = getMetaObject();
	const auto offset = findInfo<int>(metaObject, QTJSON_PROPERTY_OFFSET_KEY, 0);
	typename DataValueInfo<TValue>::Map map;
	for (auto i = offset; i < metaObject->propertyCount(); ++i) {
		const auto property = metaObject->property(i);
		if (!config.ignoreStored && !property.isStored())
			continue;

        typename DataValueInfo<TValue>::Value value;
        if constexpr (std::is_same_v<TValue, QCborValue>)
            value = propToCbor(metaObject, property, config);
        else
            value = propToJson(metaObject, property, config);

        if (value == DataValueInfo<TValue>::Invalid) {
            const auto variant = property.readOnGadget(this);
            if (!variant.isValid())
                continue;

            if (property.isEnumType()) {
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
        }

		map.insert(QString::fromUtf8(property.name()), value);
	}
	return map;
}

template<typename TValue>
void SerializableGadget::deserialize(const typename DataValueInfo<TValue>::Map &map, const Configuration &config)
{
	const auto metaObject = getMetaObject();
	const auto offset = findInfo<int>(metaObject, QTJSON_PROPERTY_OFFSET_KEY, 0);

	QStringList allKeys;
	if (config.validation.testFlag(ValidationFlag::NoExtra))
		allKeys = DataValueInfo<TValue>::keys(map);

	for (auto i = offset; i < metaObject->propertyCount(); ++i) {
		const auto property = metaObject->property(i);
		if (!config.ignoreStored && !property.isStored())
			continue;

		const auto key = QString::fromUtf8(property.name());
		if (!map.contains(key)) {
			if (config.validation.testFlag(ValidationFlag::NoMissing))
				throw ValidationFailureException{property};
			else
				continue; // skip unset properties
		}

        const auto value = map.value(key);
        bool assigned;
        if constexpr (std::is_same_v<TValue, QCborValue>)
            assigned = propAssignCbor(metaObject, property, value, config);
        else
            assigned = propAssignJson(metaObject, property, value, config);

        if (!assigned) {
			QVariant variant{property.userType(), nullptr};

			if (property.isEnumType()) {
				if (value.type() == DataValueInfo<TValue>::String) {
					auto ok = true;
					const auto key = value.toString().toUtf8();
					const auto metaEnum = property.enumerator();
					if (metaEnum.isFlag()) {
						if (key.isEmpty())
							variant = 0;
						else
							variant = metaEnum.keysToValue(key.constData(), &ok);
					} else
						variant = metaEnum.keyToValue(key.constData(), &ok);

					if (!ok)
						throw InvalidPropertyValueException{property, value};
				} else if (value.type() == DataValueInfo<TValue>::Integer) {
					if constexpr(std::is_same_v<TValue, QCborValue>)
                        variant = static_cast<int>(value.toInteger());
                    else {
                        // TODO validate is truly int
                        variant = value.toInt();
                    }
				} else
					throw InvalidValueTypeException{value.type(), {DataValueInfo<TValue>::String, DataValueInfo<TValue>::Integer}};
			} else
				variant = value.toVariant();

			if (!property.writeOnGadget(this, variant))
				throw InvalidPropertyValueException{property, value};
		}

		allKeys.removeOne(key);
	}

	if (!allKeys.isEmpty())
		throw ValidationFailureException{metaObject, allKeys};
}
