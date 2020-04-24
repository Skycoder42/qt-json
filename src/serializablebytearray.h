#pragma once

#include "iserializable.h"

#include <QByteArray>

namespace QtJson {

class QTJSON_EXPORT SerializableByteArray : public QByteArray, public ISerializable
{
public:
	using QByteArray::QByteArray;
	SerializableByteArray(const SerializableByteArray &) = default;
	SerializableByteArray(SerializableByteArray &&) noexcept = default;
	SerializableByteArray &operator=(const SerializableByteArray &) = default;
	SerializableByteArray &operator=(SerializableByteArray &&) noexcept = default;

	SerializableByteArray(const QByteArray &other);
	SerializableByteArray(QByteArray &&other) noexcept;
	SerializableByteArray &operator=(const QByteArray &other);
	SerializableByteArray &operator=(QByteArray &&other) noexcept;

	QJsonValue toJson(const JsonConfiguration &config) const override;
	void assignJson(const QJsonValue &value, const JsonConfiguration &config) override;
	QCborValue toCbor(const CborConfiguration &config) const override;
	void assignCbor(const QCborValue &value, const CborConfiguration &config) override;

	inline static SerializableByteArray fromJson(const QJsonValue &value, const JsonConfiguration &config) {
		SerializableByteArray data;
		data.assignJson(value, config);
		return data;
	}
	inline static SerializableByteArray fromCbor(const QCborValue &value, const CborConfiguration &config) {
		SerializableByteArray data;
		data.assignCbor(value, config);
		return data;
	}
};

}

Q_DECLARE_METATYPE(QtJson::SerializableByteArray)
Q_DECLARE_SHARED(QtJson::SerializableByteArray)
