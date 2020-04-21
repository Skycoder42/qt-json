#include "serializablewrapper.h"
using namespace QtJson;

QJsonValue SerializableWrapper<QByteArray>::toJson(const QtJson::JsonConfiguration &config) const
{
	return QString::fromUtf8(value().toBase64());
}

void SerializableWrapper<QByteArray>::assignJson(const QJsonValue &value, const QtJson::JsonConfiguration &config)
{
	setValue(QByteArray::fromBase64(value.toString().toUtf8()));
}

QCborValue SerializableWrapper<QByteArray>::toCbor(const QtJson::CborConfiguration &config) const
{
	return value();
}

void SerializableWrapper<QByteArray>::assignCbor(const QCborValue &value, const QtJson::CborConfiguration &config)
{
	setValue(value.toByteArray());
}
