#include "serializabledatetime.h"
#include "qtjson_exception.h"
#include "qtjson_helpers.h"
using namespace QtJson;

QJsonValue QtJson::SerializableAdapter<QDateTime, void>::toJson(const QDateTime &value, const Configuration &config)
{
	if (config.dateAsTimeStamp)
		return value.toUTC().toSecsSinceEpoch();
	else {
		if (value.timeSpec() == Qt::LocalTime)
			return value.toOffsetFromUtc(value.offsetFromUtc()).toString(Qt::ISODateWithMs);
		else
			return value.toString(Qt::ISODateWithMs);
	}
}

QDateTime QtJson::SerializableAdapter<QDateTime, void>::fromJson(const QJsonValue &value, const Configuration &config)
{
	Q_UNUSED(config);
	if (value.isDouble())
		return QDateTime::fromSecsSinceEpoch(value.toInt());
	else if (value.isString())
		return QDateTime::fromString(value.toString(), Qt::ISODateWithMs);
	else
		throw InvalidValueTypeException{value.type(), {QJsonValue::Double, QJsonValue::String}};
}

QCborValue QtJson::SerializableAdapter<QDateTime, void>::toCbor(const QDateTime &value, const Configuration &config)
{
	if (config.dateAsTimeStamp)
		return QCborValue{QCborKnownTags::UnixTime_t, value.toUTC().toSecsSinceEpoch()};
	else {
		if (value.timeSpec() == Qt::LocalTime)
			return QCborValue{value.toOffsetFromUtc(value.offsetFromUtc())};
		else
			return QCborValue{value};
	}
}

QDateTime QtJson::SerializableAdapter<QDateTime, void>::fromCbor(const QCborValue &value, const Configuration &config)
{
	Q_UNUSED(config);
	const auto xValue = __private::extract(value, nullptr, true);
	if (!xValue.isDateTime())
		throw InvalidValueTypeException{xValue.type(), {QCborValue::DateTime}};
	return __private::extract(value, nullptr, true).toDateTime();
}
