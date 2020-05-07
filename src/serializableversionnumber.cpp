#include "serializableversionnumber.h"
#include "serializableadapter.h"
#include "serializablearray.h"
using namespace QtJson;

SerializableVersionNumber::SerializableVersionNumber(const QVersionNumber &other) :
    QVersionNumber{other}
{}

SerializableVersionNumber::SerializableVersionNumber(QVersionNumber &&other) noexcept :
    QVersionNumber{std::move(other)}
{}

SerializableVersionNumber &SerializableVersionNumber::operator=(const QVersionNumber &other)
{
    QVersionNumber::operator=(other);
    return *this;
}

SerializableVersionNumber &SerializableVersionNumber::operator=(QVersionNumber &&other) noexcept
{
    QVersionNumber::operator=(std::move(other));
    return *this;
}

QJsonValue SerializableVersionNumber::toJson(const QtJson::Configuration &config) const
{
    if (config.versionAsString)
        return toString();
    else
        return SerializableVector<int>{segments()}.toJson(config);
}

void SerializableVersionNumber::assignJson(const QJsonValue &value, const QtJson::Configuration &config)
{
    if (value.isString())
        operator=(fromString(value.toString()));
    else
        operator=(QVersionNumber{SerializableVector<int>::fromJson(value, config)});
}

QCborValue SerializableVersionNumber::toCbor(const QtJson::Configuration &config) const
{
    if (config.versionAsString)
        return toString();
    else
        return SerializableVector<int>{segments()}.toCbor(config);
}

void SerializableVersionNumber::assignCbor(const QCborValue &value, const QtJson::Configuration &config)
{
    if (value.isString())
        operator=(fromString(value.toString()));
    else
        operator=(QVersionNumber{SerializableVector<int>::fromCbor(value, config)});
}
