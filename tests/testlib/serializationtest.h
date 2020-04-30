#pragma once

#include <QtTest>

#include "iserializable.h"
#include "qtjson_exception.h"

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

protected:
    using SerializationTestBase::compare;
    void compare(const TSerializable &lhs, const TSerializable &rhs, const char *file, int line) const;
};



template <typename TSerializable>
void SerializationTest<TSerializable>::testSerialization_data()
{
    QTest::addColumn<QtJson::CommonConfiguration>("config");
    QTest::addColumn<TSerializable>("data");
    QTest::addColumn<QJsonValue>("json");
    QTest::addColumn<QCborValue>("cbor");
    QTest::addColumn<bool>("throws");

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
    QFETCH(bool, throws);

    try {
        if (throws) {
            if (!json.isUndefined())
                QVERIFY_EXCEPTION_THROWN(data.toJson(config), QtJson::Exception);
            if (!cbor.isInvalid())
                QVERIFY_EXCEPTION_THROWN(data.toCbor(config), QtJson::Exception);
        } else {
            if (!json.isUndefined())
                compare(data.toJson(config), json, __FILE__, __LINE__);
            if (!cbor.isInvalid())
                compare(data.toCbor(config), cbor, __FILE__, __LINE__);
        }
    } catch (std::exception &e) {
        QFAIL(e.what());
    }
}

template <typename TSerializable>
void SerializationTest<TSerializable>::testDeserialization_data()
{
    QTest::addColumn<QtJson::CommonConfiguration>("config");
    QTest::addColumn<TSerializable>("data");
    QTest::addColumn<QJsonValue>("json");
    QTest::addColumn<QCborValue>("cbor");
    QTest::addColumn<bool>("throws");

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
    QFETCH(bool, throws);

    try {
        if (throws) {
            if (!json.isUndefined())
                QVERIFY_EXCEPTION_THROWN(TSerializable::fromJson(json, config), QtJson::Exception);
            if (!cbor.isInvalid())
                QVERIFY_EXCEPTION_THROWN(TSerializable::fromCbor(cbor, config), QtJson::Exception);
        } else {
            if (!json.isUndefined())
                compare(TSerializable::fromJson(json, config), data, __FILE__, __LINE__);
            if (!cbor.isInvalid())
                compare(TSerializable::fromCbor(cbor, config), data, __FILE__, __LINE__);
        }
    } catch (std::exception &e) {
        QFAIL(e.what());
    }
}

template<typename TSerializable>
void SerializationTest<TSerializable>::compare(const TSerializable &lhs, const TSerializable &rhs, const char *file, int line) const
{
    const auto lStr = QVariant{lhs}.toString();
    const auto rStr = QVariant{rhs}.toString();
    if (!QTest::qCompare<TSerializable, TSerializable>(lhs, rhs, qUtf8Printable(lStr), qUtf8Printable(rStr), file, line)){
        qCritical().noquote() << "Actual:  " << lStr;
        qCritical().noquote() << "Expected:" << rStr;
    }
}
