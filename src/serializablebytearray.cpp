#include "serializablebytearray.h"
using namespace QtJson;

SerializableByteArray::SerializableByteArray(const QByteArray &other) :
	QByteArray{other}
{}

SerializableByteArray::SerializableByteArray(QByteArray &&other) noexcept :
	QByteArray{std::move(other)}
{}

SerializableByteArray &SerializableByteArray::operator=(const QByteArray &other)
{
	QByteArray::operator=(other);
	return *this;
}

SerializableByteArray &SerializableByteArray::operator=(QByteArray &&other) noexcept
{
	QByteArray::operator=(std::move(other));
	return *this;
}

QJsonValue SerializableByteArray::toJson(const QtJson::JsonConfiguration &config) const
{
	switch (config.byteArrayMode) {
	case ByteArrayMode::Base64:
		return QString::fromUtf8(toBase64(Base64Encoding | KeepTrailingEquals));
	case ByteArrayMode::Base64url:
		return QString::fromUtf8(toBase64(Base64UrlEncoding | OmitTrailingEquals));
	case ByteArrayMode::Hex:
		return QString::fromUtf8(toHex());
	default:
		Q_UNREACHABLE();
	}
}

void SerializableByteArray::assignJson(const QJsonValue &value, const QtJson::JsonConfiguration &config)
{
	switch (config.byteArrayMode) {
	case ByteArrayMode::Base64:
		operator=(fromBase64(value.toString().toUtf8(), Base64Encoding));
		break;
	case ByteArrayMode::Base64url:
		operator=(fromBase64(value.toString().toUtf8(), Base64UrlEncoding));
		break;
	case ByteArrayMode::Hex:
		operator=(fromHex(value.toString().toUtf8()));
		break;
	default:
		Q_UNREACHABLE();
	}
}

QCborValue SerializableByteArray::toCbor(const QtJson::CborConfiguration &config) const
{
	switch (config.byteArrayMode) {
	case ByteArrayMode::Base64:
		return {QCborKnownTags::ExpectedBase64, QCborValue{*this}};
	case ByteArrayMode::Base64url:
		return {QCborKnownTags::ExpectedBase64url, QCborValue{*this}};
	case ByteArrayMode::Hex:
		return {QCborKnownTags::ExpectedBase16, QCborValue{*this}};
	default:
		Q_UNREACHABLE();
	}
}

void SerializableByteArray::assignCbor(const QCborValue &value, const QtJson::CborConfiguration &config)
{
	Q_UNUSED(config);
	operator=(value.toByteArray());
}
