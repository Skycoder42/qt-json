#pragma once

#include <QtTest>

#include "iserializable.h"

Q_DECLARE_METATYPE(QtJson::CommonConfiguration)

class SerializationTestBase : public QObject
{
    Q_OBJECT

protected:
    virtual void setupData() const;
    virtual void setupSerData() const;
    virtual void setupDeserData() const;

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
        QCOMPARE(data.toJson(config), json);
    if (!cbor.isInvalid())
        QCOMPARE(data.toCbor(config), cbor);
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
