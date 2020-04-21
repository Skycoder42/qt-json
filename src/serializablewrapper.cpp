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
