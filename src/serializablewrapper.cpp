#include "serializablewrapper.h"
using namespace QtJson;

QJsonValue QByteArraySerializableWrapper::toJson(const QtJson::JsonConfiguration &config) const
{
	return QString::fromUtf8(value().toBase64());
}

void QByteArraySerializableWrapper::assignJson(const QJsonValue &value, const QtJson::JsonConfiguration &config)
{
	setValue(QByteArray::fromBase64(value.toString().toUtf8()));
}

QCborValue QByteArraySerializableWrapper::toCbor(const QtJson::CborConfiguration &config) const
{
	return value();
}

void QByteArraySerializableWrapper::assignCbor(const QCborValue &value, const QtJson::CborConfiguration &config)
{
	setValue(value.toByteArray());
}
