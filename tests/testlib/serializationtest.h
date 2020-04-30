#pragma once

#include <QtTest>

#include "iserializable.h"

class SerializationTestBase : public QObject
{
    Q_OBJECT

protected:
    virtual void setupData() const;
    virtual void setupSerData() const;
    virtual void setupDeserData() const;

    void compare(const QJsonValue &lhs, const QJsonValue &rhs, const char *file, int line) const;
    void compare(const QCborValue &lhs, const QCborValue &rhs, const char *file, int line) const;

    QByteArray stringiy(const QJsonValue &value) const;

private Q_SLOTS:
    virtual void testSerialization_data() = 0;
    virtual void testSerialization() = 0;
    virtual void testDeserialization_data() = 0;
    virtual void testDeserialization() = 0;
};

template <typename TSerializable>
class SerializationTest : public SerializationTestBase
{
private:
    void testSerialization_data() final;
    void testSerialization() final;
    void testDeserialization_data() final;
    void testDeserialization() final;
};



template <typename TSerializable>
void SerializationTest<TSerializable>::testSerialization_data()
{
    QTest::addColumn<QtJson::CommonConfiguration>("config");
    QTest::addColumn<TSerializable>("data");
    QTest::addColumn<QJsonValue>("json");
    QTest::addColumn<QCborValue>("cbor");

    setupData();
    setupSerData();
}

template <typename TSerializable>
void SerializationTest<TSerializable>::testSerialization()
{
    QFETCH(QtJson::CommonConfiguration, config);
    QFETCH(TSerializable, data);
    QFETCH(QJsonValue, json);
    QFETCH(QCborValue, cbor);

    if (!json.isUndefined())
        compare(data.toJson(config), json, __FILE__, __LINE__);
    if (!cbor.isInvalid())
        compare(data.toCbor(config), cbor, __FILE__, __LINE__);
}

template <typename TSerializable>
void SerializationTest<TSerializable>::testDeserialization_data()
{
    QTest::addColumn<QtJson::CommonConfiguration>("config");
    QTest::addColumn<TSerializable>("data");
    QTest::addColumn<QJsonValue>("json");
    QTest::addColumn<QCborValue>("cbor");

    setupData();
    setupDeserData();
}

template <typename TSerializable>
void SerializationTest<TSerializable>::testDeserialization()
{
    QFETCH(QtJson::CommonConfiguration, config);
    QFETCH(TSerializable, data);
    QFETCH(QJsonValue, json);
    QFETCH(QCborValue, cbor);

    if (!json.isUndefined())
        QCOMPARE(TSerializable::fromJson(json, config), data);
    if (!cbor.isInvalid())
        QCOMPARE(TSerializable::fromCbor(cbor, config), data);
}
