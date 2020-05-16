#include <QtTest>
#include <serializationtest.h>
#include <serializableversionnumber.h>
#include <serializablearray.h>
using namespace QtJson;

class VersionNumberTest : public SerializationTest<QVersionNumber>
{
	Q_OBJECT

protected:
	void setupData() const override;
    void setupDeserData() const override;

private:
	inline Configuration c(bool versionAsString) const {
		Configuration config;
		config.versionAsString = versionAsString;
		return config;
    }
};

void VersionNumberTest::setupData() const
{
	QTest::addRow("string") << c(true)
                            << d(1, 2, 3)
							<< QJsonValue{QStringLiteral("1.2.3")}
							<< QCborValue{QStringLiteral("1.2.3")}
							<< false;
	QTest::addRow("array") << c(false)
                           << d(1, 2, 3)
						   << QJsonValue{QJsonArray{1, 2, 3}}
						   << QCborValue{HomogeneousArrayTag, QCborArray{1, 2, 3}}
                           << false;
}

void VersionNumberTest::setupDeserData() const
{
    QTest::addRow("invalid") << c(true)
                             << d()
                             << QJsonValue{12}
                             << QCborValue{12}
                             << true;
}

QTEST_APPLESS_MAIN(VersionNumberTest)

#include "tst_versionnumber.moc"
