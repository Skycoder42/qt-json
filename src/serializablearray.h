#pragma once

#include "qtjson_global.h"
#include "iserializable.h"
#include "serializableadapter.h"
#include "qtjson_exception.h"
#include "qtjson_helpers.h"
#include "serializablearray_p.h"

#include <QtCore/QtContainerFwd>
#include <QtCore/QJsonArray>
#include <QtCore/QCborArray>

namespace QtJson {

// TODO move to enum
constexpr QCborTag HomogeneousArrayTag = static_cast<QCborTag>(41);
constexpr QCborTag FiniteSetTag = static_cast<QCborTag>(258);

template <typename TValue, template <typename...> class TArray, typename... TExtra>
class SerializableAdapter<TArray<TValue, TExtra...>, std::enable_if_t<__private::is_array_v<TArray>, void>>
{
public:
	using array_type = TArray<TValue, TExtra...>;

	static QJsonValue toJson(const array_type &value, const Configuration &config = {}) {
		QJsonArray array;
		for (const auto &element : value)
			array.push_back(SerializableAdapter<TValue>::toJson(element, config));
		return array;
	}

	static array_type fromJson(const QJsonValue &value, const Configuration &config = {}) {
		if (!value.isArray())
			throw InvalidValueTypeException{value.type(), {QJsonValue::Array}};
		const auto array = value.toArray();
		array_type result;
		if constexpr (__private::has_reserve_v<array_type>)
			result.reserve(array.size());
		for (const auto &element : array)
			result.append(SerializableAdapter<TValue>::fromJson(element, config));
		return result;
	}

	static QCborValue toCbor(const array_type &value, const Configuration &config = {}) {
		QCborArray array;
		for (const auto &element : value)
			array.push_back(SerializableAdapter<TValue>::toCbor(element, config));
		return {HomogeneousArrayTag, array};
	}

	static array_type fromCbor(const QCborValue &value, const Configuration &config = {}) {
		const auto xValue = __private::extract(value);
		if (!xValue.isArray())
			throw InvalidValueTypeException{xValue.type(), {QCborValue::Array}};
		const auto array = xValue.toArray();
		array_type result;
		if constexpr (__private::has_reserve_v<array_type>)
			result.reserve(array.size());
		for (const auto &element : array)
			result.append(SerializableAdapter<TValue>::fromCbor(element, config));
		return result;
	}
};

template <typename TValue>
class SerializableAdapter<QSet<TValue>, void>
{
public:
	using array_type = QSet<TValue>;

	static QJsonValue toJson(const array_type &value, const Configuration &config = {}) {
		QJsonArray array;
		for (const auto &element : value)
			array.push_back(SerializableAdapter<TValue>::toJson(element, config));
		return array;
	}

	static array_type fromJson(const QJsonValue &value, const Configuration &config = {}) {
		if (!value.isArray())
			throw InvalidValueTypeException{value.type(), {QJsonValue::Array}};
		array_type result;
		for (const auto &element : value.toArray())
			result.insert(SerializableAdapter<TValue>::fromJson(element, config));
		return result;
	}

	static QCborValue toCbor(const array_type &value, const Configuration &config = {}) {
		QCborArray array;
		for (const auto &element : value)
			array.push_back(SerializableAdapter<TValue>::toCbor(element, config));
		return {FiniteSetTag, array};
	}

	static array_type fromCbor(const QCborValue &value, const Configuration &config = {}) {
		QCborTag tag = static_cast<QCborTag>(-1);
		const auto xValue = __private::extract(value, &tag);
		if (tag != FiniteSetTag)
			throw InvalidValueTagException{tag, {FiniteSetTag}};
		if (!xValue.isArray())
			throw InvalidValueTypeException{xValue.type(), {QCborValue::Array}};
		array_type result;
		for (const auto &element : xValue.toArray())
			result.insert(SerializableAdapter<TValue>::fromCbor(element, config));
		return result;
	}
};

}
