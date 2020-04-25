#pragma once

#include "iserializable.h"
#include "serializableadapter.h"

#include <QtCore/QList>
#include <QtCore/QJsonArray>
#include <QtCore/QCborArray>

namespace QtJson {

template <typename TValue, template <typename> class TList = QList>
class SerializableList : public TList<TValue>, public ISerializable
{
public:
	using TList<TValue>::TList;
	SerializableList(const SerializableList &) = default;
	SerializableList(SerializableList &&) noexcept = default;
	SerializableList &operator=(const SerializableList &) = default;
	SerializableList &operator=(SerializableList &&) noexcept = default;

	inline SerializableList(const TList<TValue> &other) :
		TList<TValue>{other}
	{}

	inline SerializableList(TList<TValue> &&other) noexcept :
		TList<TValue>{std::move(other)}
	{}

	inline SerializableList &operator=(const TList<TValue> &other) {
		TList<TValue>::operator=(other);
		return *this;
	}

	inline SerializableList &operator=(TList<TValue> &&other) noexcept {
		TList<TValue>::operator=(std::move(other));
		return *this;
	}

	QJsonValue toJson(const JsonConfiguration &config) const override {
		QJsonArray array;
		for (const auto &value : qAsConst(*this))
			array.append(SerializableAdapter<TValue>::toJson(value, config));
		return array;
	}

	void assignJson(const QJsonValue &value, const JsonConfiguration &config) override {
		const auto array = value.toArray();
		this->reserve(array.size());
		for (const auto &element : array)
			this->append(SerializableAdapter<TValue>::fromJson(element, config));
	}

	QCborValue toCbor(const CborConfiguration &config) const override {
		QCborArray array;
		for (const auto &value : qAsConst(*this))
			array.append(SerializableAdapter<TValue>::toCbor(value, config));
		return array;
	}

	void assignCbor(const QCborValue &value, const CborConfiguration &config) override {
		const auto array = value.toArray();
		this->reserve(array.size());
		for (const auto &element : array)
			this->append(SerializableAdapter<TValue>::fromCbor(element, config));
	}

	inline static SerializableList fromJson(const QJsonValue &value, const JsonConfiguration &config) {
		SerializableList data;
		data.assignJson(value, config);
		return data;
	}

	inline static SerializableList fromCbor(const QCborValue &value, const CborConfiguration &config) {
		SerializableList data;
		data.assignCbor(value, config);
		return data;
	}
};

// TODO container specializations

}

