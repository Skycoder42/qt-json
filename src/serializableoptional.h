#pragma once

#include <optional>

#include "iserializable.h"
#include "serializableadapter.h"
#include "qtjson_helpers.h"

namespace QtJson {

template <typename TValue>
class SerializableOptional : public std::optional<TValue>, public ISerializable
{
public:
	using std::optional<TValue>::optional;
	SerializableOptional(const SerializableOptional &) = default;
	SerializableOptional(SerializableOptional &&) noexcept = default;
	SerializableOptional &operator=(const SerializableOptional &) = default;
	SerializableOptional &operator=(SerializableOptional &&) noexcept = default;

	inline SerializableOptional(const std::optional<TValue> &other) :
		std::optional<TValue>{other}
	{}

	inline SerializableOptional(std::optional<TValue> &&other) noexcept :
		std::optional<TValue>{std::move(other)}
	{}

	inline SerializableOptional &operator=(const std::optional<TValue> &other) {
		std::optional<TValue>::operator=(other);
		return *this;
	}

	inline SerializableOptional &operator=(std::optional<TValue> &&other) noexcept {
		std::optional<TValue>::operator=(std::move(other));
		return *this;
	}

	inline bool operator==(const SerializableOptional &other) const {
		return static_cast<std::optional<TValue>>(*this) == static_cast<std::optional<TValue>>(other);
	}

	QJsonValue toJson(const Configuration &config) const override {
		if (this->has_value())
			return SerializableAdapter<TValue>::toJson(this->value(), config);
		else
			return QJsonValue::Null;
	}

	void assignJson(const QJsonValue &value, const Configuration &config) override {
		if (value.isNull())
			this->std::optional<TValue>::operator=(std::nullopt);
		else
			this->std::optional<TValue>::operator=(SerializableAdapter<TValue>::fromJson(value, config));
	}

	QCborValue toCbor(const Configuration &config) const override {
		if (this->has_value())
			return SerializableAdapter<TValue>::toCbor(this->value(), config);
		else
			return QCborSimpleType::Null;
	}

	void assignCbor(const QCborValue &value, const Configuration &config) override {
		if (__private::extract(value).isNull())
			this->std::optional<TValue>::operator=(std::nullopt);
		else
			this->std::optional<TValue>::operator=(SerializableAdapter<TValue>::fromCbor(value, config));
	}

	inline static SerializableOptional fromJson(const QJsonValue &value, const Configuration &config) {
		SerializableOptional data;
		data.assignJson(value, config);
		return data;
	}

	inline static SerializableOptional fromCbor(const QCborValue &value, const Configuration &config) {
		SerializableOptional data;
		data.assignCbor(value, config);
		return data;
	}
};

}
