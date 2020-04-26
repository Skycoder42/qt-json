#pragma once

#include "iserializable.h"
#include "serializableadapter.h"

#include <QtCore/QtContainerFwd>
#include <QtCore/QJsonObject>
#include <QtCore/QCborMap>

namespace QtJson {

template <typename TKey, typename TValue, template <typename, typename> class TDict>
class SerializableDictionary: public TDict<TKey, TValue>, public ISerializable
{
public:
public:
	using TDict<TKey, TValue>::TDict;
	SerializableDictionary(const SerializableDictionary &) = default;
	SerializableDictionary(SerializableDictionary &&) noexcept = default;
	SerializableDictionary &operator=(const SerializableDictionary &) = default;
	SerializableDictionary &operator=(SerializableDictionary &&) noexcept = default;

	inline SerializableDictionary(const TDict<TKey, TValue> &other) :
		TDict<TKey, TValue>{other}
	{}

	inline SerializableDictionary(TDict<TKey, TValue> &&other) noexcept :
		TDict<TKey, TValue>{std::move(other)}
	{}

	inline SerializableDictionary &operator=(const TDict<TKey, TValue> &other) {
		TDict<TKey, TValue>::operator=(other);
		return *this;
	}

	inline SerializableDictionary &operator=(TDict<TKey, TValue> &&other) noexcept {
		TDict<TKey, TValue>::operator=(std::move(other));
		return *this;
	}

	QJsonValue toJson(const JsonConfiguration &config) const override {
		QJsonObject object;
		for (auto it = this->begin(), end = this->end(); it != end; ++it) {
			object.insert(QVariant::fromValue(it.key()),
						  SerializableAdapter<TValue>::toJson(it.value(), config));
		}
		return object;
	}

	void assignJson(const QJsonValue &value, const JsonConfiguration &config) override {
		const auto object = value.toObject();
		for (auto it = object.begin(), end = object.end(); it != end; ++it) {
			insert(QVariant{it.key()}.template value<TKey>(),
				   SerializableAdapter<TValue>::fromJson(it.value(), config));
		}
	}

	QCborValue toCbor(const CborConfiguration &config) const override {
		QCborMap map;
		for (auto it = this->begin(), end = this->end(); it != end; ++it) {
			map.insert(SerializableAdapter<TKey>::toCbor(it.key(), config),
					   SerializableAdapter<TValue>::toCbor(it.value(), config));
		}
		return map;
	}

	void assignCbor(const QCborValue &value, const CborConfiguration &config) override {
		const auto map = value.toMap();
		for (auto it = map.begin(), end = map.end(); it != end; ++it) {
			insert(SerializableAdapter<TKey>::fromCbor(it.key(), config),
				   SerializableAdapter<TValue>::fromCbor(it.value(), config));
		}
	}

	inline static SerializableDictionary fromJson(const QJsonValue &value, const JsonConfiguration &config) {
		SerializableDictionary data;
		data.assignJson(value, config);
		return data;
	}

	inline static SerializableDictionary fromCbor(const QCborValue &value, const CborConfiguration &config) {
		SerializableDictionary data;
		data.assignCbor(value, config);
		return data;
	}
};

template <typename TKey, typename TValue>
using SerializableHash = SerializableDictionary<TKey, TValue, QHash>;
template <typename TKey, typename TValue>
using SerializableMap = SerializableDictionary<TKey, TValue, QMap>;

}

Q_DECLARE_ASSOCIATIVE_CONTAINER_METATYPE(QtJson::SerializableHash)
Q_DECLARE_ASSOCIATIVE_CONTAINER_METATYPE(QtJson::SerializableMap)
