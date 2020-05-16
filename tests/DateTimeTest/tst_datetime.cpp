#include <QtTest>
#include <serializabledatetime.h>
#include <serializationtest.h>
using namespace QtJson;

class DateTimeTest : public SerializationTest<QDateTime>
{
	Q_OBJECT

protected:
	void setupData() const override;
	void setupSerData() const override;
	void setupDeserData() const override;

private:
	inline Configuration c(bool dateAsTimeStamp) const {
		Configuration config;
		config.dateAsTimeStamp = dateAsTimeStamp;
		return config;
	}

	inline QString timeZoneOffset(int localOffset, bool useZ = true) const {
		if (localOffset == 0 && useZ)
			return QStringLiteral("Z");
		else {
			return QStringLiteral("+%1:%2")
				.arg(localOffset / 3600, 2, 10, QLatin1Char('0'))
				.arg((localOffset / 60) % 60, 2, 10, QLatin1Char('0'));
		}
	}
};

void DateTimeTest::setupData() const
{
	const auto localOffset = QDateTime::currentDateTime().offsetFromUtc();
	const auto offsetStr = timeZoneOffset(localOffset);

	QDateTime dt{{2020, 10, 10}, {17, 24, 30, 123}};
	QTest::addRow("string.local") << c(false)
                                  << d(dt)
								  << QJsonValue{QStringLiteral("2020-10-10T17:24:30.123") + offsetStr}
								  << QCborValue{
										 QCborKnownTags::DateTimeString,
										 QStringLiteral("2020-10-10T17:24:30.123") + offsetStr
									 }
								  << false;

	dt = {{2020, 10, 10}, {17, 24, 30, 123}, Qt::UTC};
	QTest::addRow("string.utc") << c(false)
                                << d(dt)
								<< QJsonValue{QStringLiteral("2020-10-10T17:24:30.123Z")}
								<< QCborValue{
									   QCborKnownTags::DateTimeString,
									   QStringLiteral("2020-10-10T17:24:30.123Z")
								   }
								<< false;

	dt = {{2020, 10, 10}, {17, 24, 30, 123}, Qt::OffsetFromUTC, 2700};
	QTest::addRow("string.offset") << c(false)
                                   << d(dt)
								   << QJsonValue{QStringLiteral("2020-10-10T17:24:30.123") + timeZoneOffset(2700)}
								   << QCborValue{
										  QCborKnownTags::DateTimeString,
										  QStringLiteral("2020-10-10T17:24:30.123") + timeZoneOffset(2700)
									  }
								   << false;

	dt = {{2020, 10, 10}, {17, 24, 30, 123}, QTimeZone::systemTimeZone()};
	QTest::addRow("string.timezone") << c(false)
                                     << d(dt)
									 << QJsonValue{QStringLiteral("2020-10-10T17:24:30.123") + timeZoneOffset(localOffset, false)}
									 << QCborValue{dt}
									 << false;

	dt = {{2020, 10, 10}, {17, 24, 30}};
	QTest::addRow("tstamp.local") << c(true)
                                  << d(dt)
								  << QJsonValue{1602350670 - localOffset}
								  << QCborValue{
										 QCborKnownTags::UnixTime_t,
										 1602350670 - localOffset
									 }
								  << false;

	dt = {{2020, 10, 10}, {17, 24, 30}, Qt::UTC};
	QTest::addRow("tstamp.utc") << c(true)
                                << d(dt)
								<< QJsonValue{1602350670}
								<< QCborValue{
									   QCborKnownTags::UnixTime_t,
									   1602350670
								   }
								<< false;
}

void DateTimeTest::setupSerData() const
{
	QDateTime dt{{2020, 10, 10}, {17, 24, 30, 123}, Qt::UTC};
	QTest::addRow("tstamp.shortened") << c(true)
                                      << d(dt)
									  << QJsonValue{1602350670}
									  << QCborValue{
											 QCborKnownTags::UnixTime_t,
											 1602350670
										 }
									  << false;
}

void DateTimeTest::setupDeserData() const
{
	QTest::addRow("invalid") << c(false)
                             << d()
							 << QJsonValue{true}
							 << QCborValue{QStringLiteral("2020-10-10T17:24:30.123Z")}
                             << true;
}

QTEST_APPLESS_MAIN(DateTimeTest)

#include "tst_datetime.moc"
