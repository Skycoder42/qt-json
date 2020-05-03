#include <QtTest>
#include <testlib.h>
#include <serializableadapter.h>
using namespace QtJson;

class TestSerializable : public ISerializable
{
public:
    double value = 0;

    inline TestSerializable() = default;
    inline TestSerializable(double value) : value{value} {}

    inline bool operator==(const TestSerializable &other) const {
        return value == other.value;
    }

    inline QJsonValue toJson(const JsonConfiguration &) const override {
        return value;
    }
    inline void assignJson(const QJsonValue &value, const JsonConfiguration &) override {
        this->value = value.toDouble();
    }
    inline QCborValue toCbor(const CborConfiguration &) const override {
        return value;
    }
    inline void assignCbor(const QCborValue &value, const CborConfiguration &) override {
        this->value = value.toDouble();
    }
};

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
