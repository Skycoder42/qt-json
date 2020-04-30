#include <QtTest>
#include <serializabledatetime.h>
#include <serializationtest.h>
using namespace QtJson;

class DateTimeTest : public SerializationTest<SerializableDateTime>
{
    Q_OBJECT

protected:
    void setupData() const;

private:
    static inline SerializableDateTime d(const QDateTime &d) {
        return d;
    }
    static inline CommonConfiguration c(bool dateAsTimeStamp) {
        CommonConfiguration config;
        config.dateAsTimeStamp = dateAsTimeStamp;
        return config;
    }
};

void DateTimeTest::setupData() const
{
    const auto localOffset = QDateTime::currentDateTime().offsetFromUtc();
    QDateTime dt{{2020, 10, 10}, {17, 24, 30, 123}};
    QTest::addRow("string.local") << c(false)
                                  << d(dt)
                                  << QJsonValue{QStringLiteral("2020-10-10T17:24:30.123+%1:00").arg(localOffset / 3600, 2, 10, QLatin1Char('0'))}
                                  << QCborValue{QDateTime{dt.date(), dt.time(), Qt::OffsetFromUTC, localOffset}};
}

QTEST_APPLESS_MAIN(DateTimeTest)

#include "tst_datetime.moc"
