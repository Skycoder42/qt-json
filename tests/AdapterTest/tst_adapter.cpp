#include <QtTest>
#include <testlib.h>
#include <serializableadapter.h>
#include <testserializable.h>
using namespace QtJson;

class AdapterTest : public QObject
{
    Q_OBJECT

private slots:
    void testWrapNormalType();
    void testWrapSerializableType();
};

void AdapterTest::testWrapNormalType()
{
    QCOMPARE(SerializableAdapter<int>::toJson(42), QJsonValue{42});
    QCOMPARE(SerializableAdapter<int>::toCbor(42), QCborValue{42});
    QCOMPARE(SerializableAdapter<int>::fromJson(QJsonValue{42}), 42);
    QCOMPARE(SerializableAdapter<int>::fromCbor(QCborValue{42}), 42);
}

void AdapterTest::testWrapSerializableType()
{
    QCOMPARE(SerializableAdapter<TestSerializable>::toJson({4.2}), QJsonValue{4.2});
    QCOMPARE(SerializableAdapter<TestSerializable>::toCbor({4.2}), QCborValue{4.2});
    QCOMPARE(SerializableAdapter<TestSerializable>::fromJson(QJsonValue{4.2}), TestSerializable{4.2});
    QCOMPARE(SerializableAdapter<TestSerializable>::fromCbor(QCborValue{4.2}), TestSerializable{4.2});
}

QTEST_APPLESS_MAIN(AdapterTest)

#include "tst_adapter.moc"
