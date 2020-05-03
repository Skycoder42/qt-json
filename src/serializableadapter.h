#pragma once

#include "iserializable.h"

namespace QtJson {

template <typename TType, typename = void>
class SerializableAdapter;

template <typename TType>
class SerializableAdapter<TType, std::enable_if_t<std::is_base_of_v<ISerializable, TType>, void>>
{
public:
	static inline QJsonValue toJson(const TType &value, const CommonConfiguration &config = {}) {
		return value.toJson(config);
	}
	static inline TType fromJson(const QJsonValue &value, const CommonConfiguration &config = {}) {
		TType data;
		data.assignJson(value, config);
		return data;
	}

	static inline QCborValue toCbor(const TType &value, const CommonConfiguration &config = {}) {
		return value.toCbor(config);

	}
	static inline TType fromCbor(const QCborValue &value, const CommonConfiguration &config = {}) {
		TType data;
		data.assignCbor(value, config);
		return data;
	}
};

template <typename TType>
class SerializableAdapter<TType, std::enable_if_t<!std::is_base_of_v<ISerializable, TType>, void>>
{
public:
    static inline QJsonValue toJson(const TType &value, const CommonConfiguration & = {}) {
		return QJsonValue::fromVariant(QVariant::fromValue(value));
	}
    static inline TType fromJson(const QJsonValue &value, const CommonConfiguration & = {}) {
		return value.toVariant().template value<TType>();
	}

    static inline QCborValue toCbor(const TType &value, const CommonConfiguration & = {}) {
		return QCborValue::fromVariant(QVariant::fromValue(value));
	}
    static inline TType fromCbor(const QCborValue &value, const CommonConfiguration & = {}) {
		return value.toVariant().template value<TType>();
	}
};

}
