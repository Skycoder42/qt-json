#include <QtTest>
#include <serializablebytearray.h>
#include <serializationtest.h>
using namespace QtJson;

class ByteArrayTest : public SerializationTest<SerializableByteArray>
{
    Q_OBJECT

protected:
    void setupData() const;
};

void ByteArrayTest::setupData() const
{
    CommonConfiguration c;
    c.byteArrayMode = ByteArrayMode::Base64;
    QTest::addRow("base64") << c
                            << SerializableByteArray{QByteArray{"test"}}
                            << QJsonValue{QStringLiteral("dGVzdA==")}
                            << QCborValue{QCborKnownTags::ExpectedBase64, QByteArray{"test"}};
}

QTEST_APPLESS_MAIN(ByteArrayTest)

#include "tst_bytearray.moc"
