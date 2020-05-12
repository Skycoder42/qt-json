#pragma once

#include <optional>

#include "iserializable.h"
#include "serializableadapter.h"
#include "qtjson_helpers.h"

namespace QtJson {

template <typename TValue>
class SerializableAdapter<std::optional<TValue>, void>
{
public:
	static QJsonValue toJson(const std::optional<TValue> &value, const Configuration &config = {}) {
		if (value)
			return SerializableAdapter<TValue>::toJson(*value, config);
		else
			return QJsonValue::Null;
	}

	static std::optional<TValue> fromJson(const QJsonValue &value, const Configuration &config = {}) {
		if (value.isNull())
			return std::nullopt;
		else
			return SerializableAdapter<TValue>::fromJson(value, config);
	}

	static QCborValue toCbor(const std::optional<TValue> &value, const Configuration &config = {}) {
		if (value)
			return SerializableAdapter<TValue>::toCbor(*value, config);
		else
			return QCborSimpleType::Null;
	}

	static std::optional<TValue> fromCbor(const QCborValue &value, const Configuration &config = {}) {
		if (__private::extract(value).isNull())
			return std::nullopt;
		else
			return SerializableAdapter<TValue>::fromCbor(value, config);
	}
};

}
