#pragma once

#include <optional>

#include "iserializable.h"
#include "serializableadapter.h"

namespace QtJson {

template <typename TValue>
class SerializableOptional : public std::optional<TValue>, public ISerializable
{
public:
    using std::optional<TValue>::TList;
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

    QJsonValue toJson(const CommonConfiguration &config) const override {
        if (this->has_value())
            return SerializableAdapter<TValue>::toJson(this->value(), config);
        else
            return QJsonValue::Null;
    }

    void assignJson(const QJsonValue &value, const CommonConfiguration &config) override {
        if (value.isNull())
            this->operator=(std::nullopt);
        else
            this->operator=(SerializableAdapter<TValue>::fromJson(value, config));
    }

    QCborValue toCbor(const CommonConfiguration &config) const override {
        if (this->has_value())
            return SerializableAdapter<TValue>::toCbor(this->value(), config);
        else
            return QCborSimpleType::Null;
    }

    void assignCbor(const QCborValue &value, const CommonConfiguration &config) override {
        if ((value.isTag() ? value.taggedValue() : value).isNull())
            this->operator=(std::nullopt);
        else
            this->operator=(SerializableAdapter<TValue>::fromCbor(value, config));
    }

    inline static SerializableOptional fromJson(const QJsonValue &value, const CommonConfiguration &config) {
        SerializableOptional data;
        data.assignJson(value, config);
        return data;
    }

    inline static SerializableOptional fromCbor(const QCborValue &value, const CommonConfiguration &config) {
        SerializableOptional data;
        data.assignCbor(value, config);
        return data;
    }
};

}
