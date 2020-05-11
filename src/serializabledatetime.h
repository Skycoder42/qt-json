#pragma once

#include "iserializable.h"

#include <QtCore/QDateTime>

namespace QtJson {

class QTJSON_EXPORT SerializableDateTime : public QDateTime, public ISerializable
{
public:
    SerializableDateTime() = default;
	SerializableDateTime(const SerializableDateTime &) = default;
	SerializableDateTime(SerializableDateTime &&) noexcept = default;
	SerializableDateTime &operator=(const SerializableDateTime &) = default;
	SerializableDateTime &operator=(SerializableDateTime &&) noexcept = default;

	SerializableDateTime(const QDateTime &other);
	SerializableDateTime(QDateTime &&other) noexcept;
	SerializableDateTime &operator=(const QDateTime &other);
	SerializableDateTime &operator=(QDateTime &&other) noexcept;

    QJsonValue toJson(const Configuration &config = {}) const override;
    void assignJson(const QJsonValue &value, const Configuration &config = {}) override;
    QCborValue toCbor(const Configuration &config = {}) const override;
    void assignCbor(const QCborValue &value, const Configuration &config = {}) override;

    template <typename... TArgs>
    inline SerializableDateTime &emplace(TArgs&&... args) {
        return operator=(QDateTime{std::forward<TArgs>(args)...});
    }

    inline static SerializableDateTime fromJson(const QJsonValue &value, const Configuration &config = {}) {
		SerializableDateTime data;
		data.assignJson(value, config);
		return data;
	}
    inline static SerializableDateTime fromCbor(const QCborValue &value, const Configuration &config = {}) {
		SerializableDateTime data;
		data.assignCbor(value, config);
		return data;
	}
};

}

Q_DECLARE_METATYPE(QtJson::SerializableDateTime)
Q_DECLARE_SHARED(QtJson::SerializableDateTime)

