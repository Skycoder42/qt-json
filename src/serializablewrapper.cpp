#include "serializablewrapper.h"
using namespace QtJson;

QJsonValue SerializableWrapper<QByteArray>::toJson(const QtJson::JsonConfiguration &config) const
{
    switch (config.byteArrayMode) {
    case ByteArrayMode::Base64:
        return QString::fromUtf8(value().toBase64(QByteArray::Base64Encoding | QByteArray::KeepTrailingEquals));
    case ByteArrayMode::Base64url:
        return QString::fromUtf8(value().toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
    case ByteArrayMode::Hex:
        return QString::fromUtf8(value().toHex());
    default:
        Q_UNREACHABLE();
    }
}

void SerializableWrapper<QByteArray>::assignJson(const QJsonValue &value, const QtJson::JsonConfiguration &config)
{
    switch (config.byteArrayMode) {
    case ByteArrayMode::Base64:
        setValue(QByteArray::fromBase64(value.toString().toUtf8(), QByteArray::Base64Encoding));
        break;
    case ByteArrayMode::Base64url:
        setValue(QByteArray::fromBase64(value.toString().toUtf8(), QByteArray::Base64UrlEncoding));
        break;
    case ByteArrayMode::Hex:
        setValue(QByteArray::fromHex(value.toString().toUtf8()));
        break;
    default:
        Q_UNREACHABLE();
    }
}

QCborValue SerializableWrapper<QByteArray>::toCbor(const QtJson::CborConfiguration &config) const
{
    switch (config.byteArrayMode) {
    case ByteArrayMode::Base64:
        return {QCborKnownTags::ExpectedBase64, value()};
    case ByteArrayMode::Base64url:
        return {QCborKnownTags::ExpectedBase64url, value()};
    case ByteArrayMode::Hex:
        return {QCborKnownTags::ExpectedBase16, value()};
    default:
        Q_UNREACHABLE();
    }
}

void SerializableWrapper<QByteArray>::assignCbor(const QCborValue &value, const QtJson::CborConfiguration &config)
{
    Q_UNUSED(config);
	setValue(value.toByteArray());
}



QJsonValue QtJson::SerializableWrapper<QDateTime>::toJson(const JsonConfiguration &config) const
{
    if (config.dateAsTimeStamp)
        return value().toUTC().toSecsSinceEpoch();
    else {
        if (value().timeSpec() == Qt::LocalTime)
            return value().toOffsetFromUtc(value().offsetFromUtc()).toString(Qt::ISODateWithMs);
        else
            return value().toString(Qt::ISODateWithMs);
    }
}

void QtJson::SerializableWrapper<QDateTime>::assignJson(const QJsonValue &value, const JsonConfiguration &config)
{
    Q_UNUSED(config);
    if (value.isDouble())
        setValue(QDateTime::fromSecsSinceEpoch(value.toInt()));
    else
        setValue(QDateTime::fromString(value.toString(), Qt::ISODateWithMs));
}

QCborValue QtJson::SerializableWrapper<QDateTime>::toCbor(const CborConfiguration &config) const
{
    if (config.dateAsTimeStamp)
        return QCborValue{QCborKnownTags::UnixTime_t, value().toUTC().toSecsSinceEpoch()};
    else {
        if (value().timeSpec() == Qt::LocalTime)
            return QCborValue{value().toOffsetFromUtc(value().offsetFromUtc())};
        else
            return QCborValue{value()};
    }
}

void QtJson::SerializableWrapper<QDateTime>::assignCbor(const QCborValue &value, const CborConfiguration &config)
{
    Q_UNUSED(config);
    setValue(value.toDateTime());
}
