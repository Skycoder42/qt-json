#pragma once

#include "iserializable.h"

namespace QtJson {

template <typename TType, typename = void>
class SerializableAdapter;

template <typename TType>
class SerializableAdapter<TType, std::enable_if_t<std::is_base_of_v<ISerializable, TType>, void>>
{
public:
	static inline QJsonValue toJson(const TType &value, const JsonConfiguration &config = {}) {
		return value.toJson(config);
	}
	static inline TType fromJson(const QJsonValue &value, const JsonConfiguration &config = {}) {
		TType data;
		data.assignJson(value, config);
		return data;
	}

	static inline QCborValue toCbor(const TType &value, const CborConfiguration &config = {}) {
		return value.toCbor(config);

	}
	static inline TType fromCbor(const QCborValue &value, const CborConfiguration &config = {}) {
		TType data;
		data.assignCbor(value, config);
		return data;
	}
};

template <typename TType>
class SerializableAdapter<TType, std::enable_if_t<!std::is_base_of_v<ISerializable, TType>, void>>
{
public:
	static inline QJsonValue toJson(const TType &value, const JsonConfiguration &) {
		return QJsonValue::fromVariant(QVariant::fromValue(value));
	}
	static inline TType fromJson(const QJsonValue &value, const JsonConfiguration &) {
		return value.toVariant().template value<TType>();
	}

	static inline QCborValue toCbor(const TType &value, const CborConfiguration &) {
		return QCborValue::fromVariant(QVariant::fromValue(value));
	}
	static inline TType fromCbor(const QCborValue &value, const CborConfiguration &) {
		return value.toVariant().template value<TType>();
	}
};

}
