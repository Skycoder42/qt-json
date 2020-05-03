#pragma once

#include <iserializable.h>

class TestSerializable : public QtJson::ISerializable
{
public:
    double value = 0;

    TestSerializable();
    TestSerializable(double value);

    bool operator==(const TestSerializable &other) const;

    QJsonValue toJson(const QtJson::CommonConfiguration &) const override;
    void assignJson(const QJsonValue &value, const QtJson::CommonConfiguration &) override;
    QCborValue toCbor(const QtJson::CommonConfiguration &) const override;
    void assignCbor(const QCborValue &value, const QtJson::CommonConfiguration &) override;
};

QDebug operator<<(QDebug debug, const TestSerializable &value);
