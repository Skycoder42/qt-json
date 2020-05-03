#pragma once

#include <iserializable.h>

class TestSerializable : public QtJson::ISerializable
{
public:
    double value = 0;

    TestSerializable();
    TestSerializable(double value);

    bool operator==(const TestSerializable &other) const;

    QJsonValue toJson(const QtJson::JsonConfiguration &) const override;
    void assignJson(const QJsonValue &value, const QtJson::JsonConfiguration &) override;
    QCborValue toCbor(const QtJson::CborConfiguration &) const override;
    void assignCbor(const QCborValue &value, const QtJson::CborConfiguration &) override;
};

QDebug operator<<(QDebug debug, const TestSerializable &value);
