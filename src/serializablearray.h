#pragma once

#include "iserializable.h"
#include "serializableadapter.h"
#include "qtjson_exception.h"
#include "qtjson_common_p.h"

#include <QtCore/QtContainerFwd>
#include <QtCore/QJsonArray>
#include <QtCore/QCborArray>

namespace QtJson {

namespace __private {

template <typename, typename = void>
struct has_reserve : public std::false_type {};

template <typename T>
struct has_reserve<T, std::void_t<decltype(std::declval<T>().reserve(0))>> : std::true_type {};

template <typename T>
inline constexpr bool has_reserve_v = has_reserve<T>::value;

}

// TODO move to enum
constexpr QCborTag HomogeneousArrayTag = static_cast<QCborTag>(41);
constexpr QCborTag FiniteSetTag = static_cast<QCborTag>(258);

template <typename TValue, template <typename> class TList>
class SerializableArray : public TList<TValue>, public ISerializable
{
public:
	using TList<TValue>::TList;
	SerializableArray(const SerializableArray &) = default;
	SerializableArray(SerializableArray &&) noexcept = default;
	SerializableArray &operator=(const SerializableArray &) = default;
	SerializableArray &operator=(SerializableArray &&) noexcept = default;

	inline SerializableArray(const TList<TValue> &other) :
		TList<TValue>{other}
	{}

	inline SerializableArray(TList<TValue> &&other) noexcept :
		TList<TValue>{std::move(other)}
	{}

	inline SerializableArray &operator=(const TList<TValue> &other) {
		TList<TValue>::operator=(other);
		return *this;
	}

	inline SerializableArray &operator=(TList<TValue> &&other) noexcept {
		TList<TValue>::operator=(std::move(other));
		return *this;
	}

	QJsonValue toJson(const CommonConfiguration &config) const override {
		QJsonArray array;
		for (const auto &value : qAsConst(*this))
			array.push_back(SerializableAdapter<TValue>::toJson(value, config));
		return array;
	}

	void assignJson(const QJsonValue &value, const CommonConfiguration &config) override {
		if (!value.isArray())
			throw InvalidValueTypeException{value.type(), {QJsonValue::Array}};
		const auto array = value.toArray();
		if constexpr (__private::has_reserve_v<TList<TValue>>)
			this->reserve(array.size());
		for (const auto &element : array)
			this->append(SerializableAdapter<TValue>::fromJson(element, config));
	}

	QCborValue toCbor(const CommonConfiguration &config) const override {
		QCborArray array;
		for (const auto &value : qAsConst(*this))
			array.push_back(SerializableAdapter<TValue>::toCbor(value, config));
		return {HomogeneousArrayTag, array};
	}

	void assignCbor(const QCborValue &value, const CommonConfiguration &config) override {
		const auto xValue = __private::extract(value);
		if (!xValue.isArray())
			throw InvalidValueTypeException{xValue.type(), {QCborValue::Array}};
		const auto array = xValue.toArray();
		if constexpr (__private::has_reserve_v<TList<TValue>>)
			this->reserve(array.size());
		for (const auto &element : array)
			this->append(SerializableAdapter<TValue>::fromCbor(element, config));
	}

	inline static SerializableArray fromJson(const QJsonValue &value, const CommonConfiguration &config) {
		SerializableArray data;
		data.assignJson(value, config);
		return data;
	}

	inline static SerializableArray fromCbor(const QCborValue &value, const CommonConfiguration &config) {
		SerializableArray data;
		data.assignCbor(value, config);
		return data;
	}
};


template <typename TValue>
class SerializableArray<TValue, QSet> : public QSet<TValue>, public ISerializable
{
public:
	using QSet<TValue>::QSet;
	SerializableArray(const SerializableArray &) = default;
	SerializableArray(SerializableArray &&) noexcept = default;
	SerializableArray &operator=(const SerializableArray &) = default;
	SerializableArray &operator=(SerializableArray &&) noexcept = default;

	inline SerializableArray(const QSet<TValue> &other) :
		QSet<TValue>{other}
	{}

	inline SerializableArray(QSet<TValue> &&other) noexcept :
		QSet<TValue>{std::move(other)}
	{}

	inline SerializableArray &operator=(const QSet<TValue> &other) {
		QSet<TValue>::operator=(other);
		return *this;
	}

	inline SerializableArray &operator=(QSet<TValue> &&other) noexcept {
		QSet<TValue>::operator=(std::move(other));
		return *this;
	}

	QJsonValue toJson(const CommonConfiguration &config) const override {
		QJsonArray array;
		for (const auto &value : qAsConst(*this))
			array.append(SerializableAdapter<TValue>::toJson(value, config));
		return array;
	}

	void assignJson(const QJsonValue &value, const CommonConfiguration &config) override {
		if (!value.isArray())
			throw InvalidValueTypeException{value.type(), {QJsonValue::Array}};
		const auto array = value.toArray();
		for (const auto &element : array)
			this->insert(SerializableAdapter<TValue>::fromJson(element, config));
	}

	QCborValue toCbor(const CommonConfiguration &config) const override {
		QCborArray array;
		for (const auto &value : qAsConst(*this))
			array.append(SerializableAdapter<TValue>::toCbor(value, config));
		return {FiniteSetTag, array};
	}

	void assignCbor(const QCborValue &value, const CommonConfiguration &config) override {
		QCborTag tag = static_cast<QCborTag>(-1);
		const auto xValue = __private::extract(value, &tag);
		__private::verifyTag(tag, false, FiniteSetTag);
		if (!xValue.isArray())
			throw InvalidValueTypeException{xValue.type(), {QCborValue::Array}};
		for (const auto &element : xValue.toArray())
			this->insert(SerializableAdapter<TValue>::fromCbor(element, config));
	}

	inline static SerializableArray fromJson(const QJsonValue &value, const CommonConfiguration &config) {
		SerializableArray data;
		data.assignJson(value, config);
		return data;
	}

	inline static SerializableArray fromCbor(const QCborValue &value, const CommonConfiguration &config) {
		SerializableArray data;
		data.assignCbor(value, config);
		return data;
	}
};

template <typename TValue>
using SerializableList = SerializableArray<TValue, QList>;
template <typename TValue>
using SerializableVector = SerializableArray<TValue, QVector>;
#ifndef QT_NO_LINKED_LIST
template <typename TValue>
using SerializableLinkedList = SerializableArray<TValue, QLinkedList>;
#endif
template <typename TValue>
using SerializableStack = SerializableArray<TValue, QStack>;
template <typename TValue>
using SerializableQueue = SerializableArray<TValue, QQueue>;
template <typename TValue>
using SerializableSet = SerializableArray<TValue, QSet>;

}

Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE(QtJson::SerializableList)
Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE(QtJson::SerializableVector)
#ifndef QT_NO_LINKED_LIST
Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE(QtJson::SerializableLinkedList)
#endif
Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE(QtJson::SerializableStack)
Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE(QtJson::SerializableQueue)
Q_DECLARE_SEQUENTIAL_CONTAINER_METATYPE(QtJson::SerializableSet)
