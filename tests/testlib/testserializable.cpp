#include "testserializable.h"
using namespace QtJson;

TestSerializable::TestSerializable() = default;

TestSerializable::TestSerializable(double value) :
    value{value}
{}

bool TestSerializable::operator==(const TestSerializable &other) const
{
    return value == other.value;
}

QJsonValue TestSerializable::toJson(const CommonConfiguration &) const
{
    return value;
}

void TestSerializable::assignJson(const QJsonValue &value, const CommonConfiguration &)
{
    this->value = value.toDouble();
}

QCborValue TestSerializable::toCbor(const CommonConfiguration &) const
{
    return value;
}

void TestSerializable::assignCbor(const QCborValue &value, const CommonConfiguration &)
{
    this->value = value.toDouble();
}

QDebug operator<<(QDebug debug, const TestSerializable &value)
{
    QDebugStateSaver state{debug};
    debug.noquote().nospace() << "TestSerializable(" << value.value << ")";
    return debug;
}
