#pragma once

#include <QtCore/QVersionNumber>

#include "iserializable.h"

namespace QtJson {

class SerializableVersionNumber : public QVersionNumber, public ISerializable
{
public:
    using QVersionNumber::QVersionNumber;
    SerializableVersionNumber() = default;
    SerializableVersionNumber(const SerializableVersionNumber &) = default;
    SerializableVersionNumber(SerializableVersionNumber &&) noexcept = default;
    SerializableVersionNumber &operator=(const SerializableVersionNumber &) = default;
    SerializableVersionNumber &operator=(SerializableVersionNumber &&) noexcept = default;

    SerializableVersionNumber(const QVersionNumber &other);
    SerializableVersionNumber(QVersionNumber &&other) noexcept;
    SerializableVersionNumber &operator=(const QVersionNumber &other);
    SerializableVersionNumber &operator=(QVersionNumber &&other) noexcept;

    QJsonValue toJson(const CommonConfiguration &config = {}) const override;
    void assignJson(const QJsonValue &value, const CommonConfiguration &config = {}) override;
    QCborValue toCbor(const CommonConfiguration &config = {}) const override;
    void assignCbor(const QCborValue &value, const CommonConfiguration &config = {}) override;

    inline static SerializableVersionNumber fromJson(const QJsonValue &value, const CommonConfiguration &config = {}) {
        SerializableVersionNumber data;
        data.assignJson(value, config);
        return data;
    }
    inline static SerializableVersionNumber fromCbor(const QCborValue &value, const CommonConfiguration &config = {}) {
        SerializableVersionNumber data;
        data.assignCbor(value, config);
        return data;
    }
};

}
