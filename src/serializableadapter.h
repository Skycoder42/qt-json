#pragma once

#include "iserializable.h"

namespace QtJson {

template <typename TType, typename = void>
class SerializableAdapter
{
public:
	static inline QJsonValue toJson(const TType &value, const Configuration & = {}) {
		return QJsonValue::fromVariant(QVariant::fromValue(value));
	}
	static inline TType fromJson(const QJsonValue &value, const Configuration & = {}) {
		return value.toVariant().template value<TType>();
	}

	static inline QCborValue toCbor(const TType &value, const Configuration & = {}) {
		return QCborValue::fromVariant(QVariant::fromValue(value));
	}
	static inline TType fromCbor(const QCborValue &value, const Configuration & = {}) {
		return value.toVariant().template value<TType>();
	}
};

template <typename TType>
class SerializableAdapter<TType, std::enable_if_t<std::is_base_of_v<ISerializable, TType>, void>>
{
public:
	static inline QJsonValue toJson(const TType &value, const Configuration &config = {}) {
		return value.toJson(config);
	}
	static inline TType fromJson(const QJsonValue &value, const Configuration &config = {}) {
		TType data;
		data.assignJson(value, config);
		return data;
	}

	static inline QCborValue toCbor(const TType &value, const Configuration &config = {}) {
		return value.toCbor(config);

	}
	static inline TType fromCbor(const QCborValue &value, const Configuration &config = {}) {
		TType data;
		data.assignCbor(value, config);
		return data;
	}
};

}
