#pragma once

#include "iserializable.h"

#include <QtCore/QDateTime>

namespace QtJson {

class QTJSON_EXPORT SerializableDateTime : public QDateTime, public ISerializable
{
public:
	using QDateTime::QDateTime;
    SerializableDateTime() = default;
	SerializableDateTime(const SerializableDateTime &) = default;
	SerializableDateTime(SerializableDateTime &&) noexcept = default;
	SerializableDateTime &operator=(const SerializableDateTime &) = default;
	SerializableDateTime &operator=(SerializableDateTime &&) noexcept = default;

	SerializableDateTime(const QDateTime &other);
	SerializableDateTime(QDateTime &&other) noexcept;
	SerializableDateTime &operator=(const QDateTime &other);
	SerializableDateTime &operator=(QDateTime &&other) noexcept;

    QJsonValue toJson(const JsonConfiguration &config = {}) const override;
    void assignJson(const QJsonValue &value, const JsonConfiguration &config = {}) override;
    QCborValue toCbor(const CborConfiguration &config = {}) const override;
    void assignCbor(const QCborValue &value, const CborConfiguration &config = {}) override;

    inline static SerializableDateTime fromJson(const QJsonValue &value, const JsonConfiguration &config = {}) {
		SerializableDateTime data;
		data.assignJson(value, config);
		return data;
	}
    inline static SerializableDateTime fromCbor(const QCborValue &value, const CborConfiguration &config = {}) {
		SerializableDateTime data;
		data.assignCbor(value, config);
		return data;
	}
};

}

Q_DECLARE_METATYPE(QtJson::SerializableDateTime)
Q_DECLARE_SHARED(QtJson::SerializableDateTime)

