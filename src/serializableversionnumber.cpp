#include "serializableversionnumber.h"
#include "serializableadapter.h"
#include "serializablearray.h"
using namespace QtJson;

QJsonValue QtJson::SerializableAdapter<QVersionNumber, void>::toJson(const QVersionNumber &value, const Configuration &config)
{
	if (config.versionAsString)
		return value.toString();
	else
		return SerializableAdapter<QVector<int>>::toJson(value.segments(), config);
}

QVersionNumber QtJson::SerializableAdapter<QVersionNumber, void>::fromJson(const QJsonValue &value, const Configuration &config)
{
	if (value.isString())
		return QVersionNumber::fromString(value.toString());
	else
		return QVersionNumber{SerializableAdapter<QVector<int>>::fromJson(value, config)};
}

QCborValue QtJson::SerializableAdapter<QVersionNumber, void>::toCbor(const QVersionNumber &value, const Configuration &config)
{
	if (config.versionAsString)
		return value.toString();
	else
		return SerializableAdapter<QVector<int>>::toCbor(value.segments(), config);
}

QVersionNumber QtJson::SerializableAdapter<QVersionNumber, void>::fromCbor(const QCborValue &value, const Configuration &config)
{
	if (value.isString())
		return QVersionNumber::fromString(value.toString());
	else
		return QVersionNumber{SerializableAdapter<QVector<int>>::fromCbor(value, config)};
}
