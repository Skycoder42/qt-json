#include <QtTest>
#include <serializablebytearray.h>

Q_DECLARE_METATYPE(QtJson::ByteArrayMode)

class ByteArrayTest : public QObject
{
	Q_OBJECT

private slots:
	void serialize_data();
	void serialize();

};

void ByteArrayTest::serialize_data()
{
	QTest::addColumn<QtJson::ByteArrayMode>("mode");
	QTest::addColumn<QByteArray>("data");
	QTest::addColumn<QJsonValue>("json");
	QTest::addColumn<QCborValue>("cbor");

	QTest::addRow("base64") << QtJson::ByteArrayMode::Base64
							<< QByteArray{"test"}
							<< QJsonValue{QStringLiteral("dGVzdAo=")}
							<< QCborValue{QByteArray{"test"}};
}

void ByteArrayTest::serialize()
{
	QFETCH(QtJson::ByteArrayMode, mode);
	QFETCH(QByteArray, data);
	QFETCH(QJsonValue, json);
	QFETCH(QCborValue, cbor);

	QtJson::JsonConfiguration jConf;
	QtJson::CborConfiguration cConf;
	jConf.byteArrayMode = mode;
	cConf.byteArrayMode = mode;
	QtJson::SerializableByteArray test = data;
	QCOMPARE(test.toJson(jConf), json);
	QCOMPARE(test.toCbor(cConf), cbor);
}

QTEST_APPLESS_MAIN(ByteArrayTest)

#include "tst_bytearray.moc"
