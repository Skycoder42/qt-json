#include "serializabledatetime.h"
using namespace QtJson;

SerializableDateTime::SerializableDateTime(const QDateTime &other) :
	QDateTime{other}
{}

SerializableDateTime::SerializableDateTime(QDateTime &&other) noexcept :
	QDateTime{std::move(other)}
{}

SerializableDateTime &SerializableDateTime::operator=(const QDateTime &other)
{
	QDateTime::operator=(other);
	return *this;
}

SerializableDateTime &SerializableDateTime::operator=(QDateTime &&other) noexcept
{
	QDateTime::operator=(std::move(other));
	return *this;
}

QJsonValue SerializableDateTime::toJson(const QtJson::JsonConfiguration &config) const
{
	if (config.dateAsTimeStamp)
		return toUTC().toSecsSinceEpoch();
	else {
		if (timeSpec() == Qt::LocalTime)
			return toOffsetFromUtc(offsetFromUtc()).toString(Qt::ISODateWithMs);
		else
			return toString(Qt::ISODateWithMs);
	}
}

void SerializableDateTime::assignJson(const QJsonValue &value, const QtJson::JsonConfiguration &config)
{
	Q_UNUSED(config);
	if (value.isDouble())
		operator=(fromSecsSinceEpoch(value.toInt()));
	else
		operator=(fromString(value.toString(), Qt::ISODateWithMs));
}

QCborValue SerializableDateTime::toCbor(const QtJson::CborConfiguration &config) const
{
	if (config.dateAsTimeStamp)
		return QCborValue{QCborKnownTags::UnixTime_t, toUTC().toSecsSinceEpoch()};
	else {
		if (timeSpec() == Qt::LocalTime)
			return QCborValue{toOffsetFromUtc(offsetFromUtc())};
		else
			return QCborValue{*this};
	}
}

void SerializableDateTime::assignCbor(const QCborValue &value, const QtJson::CborConfiguration &config)
{
	Q_UNUSED(config);
	operator=(value.toDateTime());
}

