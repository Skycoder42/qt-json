#pragma once

#include <variant>
#include <optional>
#include <type_traits>
#include <QtTest>

#include "iserializable.h"
#include "serializableadapter.h"
#include "qtjson_exception.h"

Q_DECLARE_METATYPE(QSharedPointer<const QtJson::ISerializable>)

class SerializationTestBase : public QObject
{
    Q_OBJECT

protected:
	virtual void setupData() const;
	virtual void setupSerData() const;
	virtual void setupDeserData() const;

	void compare(const QJsonValue &actual, const QJsonValue &expected, const char *file, int line) const;
	void compare(const QCborValue &actual, const QCborValue &expected, const char *file, int line) const;

private Q_SLOTS:
    virtual void testSerialization_data() = 0;
    virtual void testSerialization() = 0;
    virtual void testDeserialization_data() = 0;
    virtual void testDeserialization() = 0;

private:
    QByteArray stringify(const QJsonValue &value) const;
};



template <typename TSerializable>
class SerializationTest : public SerializationTestBase
{
protected:
    using SerializationTestBase::compare;
    void compare(const TSerializable &actual, const TSerializable &expected, const char *file, int line) const;

    template <typename... TArgs>
    inline TSerializable d(TArgs&&... args) const;

private:
    void testSerialization_data() final;
    void testSerialization() final;
    void testDeserialization_data() final;
    void testDeserialization() final;
};



template <typename... TSerializables>
class SerializationMultiTest : public SerializationTestBase
{
public:
    using Variant = std::variant<TSerializables...>;

protected:
    using SerializationTestBase::compare;
    void compare(const Variant &actual, const Variant &expected, const char *file, int line) const;

    template <typename TSerializable, typename... TArgs>
    inline Variant d(TArgs&&... args) const;

private:
    void testSerialization_data() final;
    void testSerialization() final;
    void testDeserialization_data() final;
    void testDeserialization() final;
};



template<typename TSerializable>
template<typename... TArgs>
TSerializable SerializationTest<TSerializable>::d(TArgs&&... args) const
{
    return TSerializable{std::forward<TArgs>(args)...};
}

template<typename TSerializable>
void SerializationTest<TSerializable>::compare(const TSerializable &actual, const TSerializable &expected, const char *file, int line) const
{
    if (!QTest::qCompare(actual, expected, "actual", "expected", file, line)) {
        qCritical().noquote() << "Actual:  " << actual;
        qCritical().noquote() << "Expected:" << expected;
    }
}

template<typename TSerializable>
void SerializationTest<TSerializable>::testSerialization_data()
{
    QTest::addColumn<QtJson::Configuration>("config");
    QTest::addColumn<TSerializable>("data");
    QTest::addColumn<QJsonValue>("json");
    QTest::addColumn<QCborValue>("cbor");
    QTest::addColumn<bool>("throws");

    setupData();
    setupSerData();
}

template<typename TSerializable>
void SerializationTest<TSerializable>::testSerialization()
{
    QFETCH(QtJson::Configuration, config);
    QFETCH(TSerializable, data);
    QFETCH(QJsonValue, json);
    QFETCH(QCborValue, cbor);
    QFETCH(bool, throws);

    try {
        if (throws) {
            if (!json.isUndefined())
                QVERIFY_EXCEPTION_THROWN(QtJson::SerializableAdapter<TSerializable>::toJson(data, config), QtJson::Exception);
            if (!cbor.isInvalid())
                QVERIFY_EXCEPTION_THROWN(QtJson::SerializableAdapter<TSerializable>::toCbor(data, config), QtJson::Exception);
        } else {
            if (!json.isUndefined())
                compare(QtJson::SerializableAdapter<TSerializable>::toJson(data, config), json, __FILE__, __LINE__);
            if (!cbor.isInvalid())
                compare(QtJson::SerializableAdapter<TSerializable>::toCbor(data, config), cbor, __FILE__, __LINE__);
        }
    } catch (std::exception &e) {
        QFAIL(e.what());
    }
}

template<typename TSerializable>
void SerializationTest<TSerializable>::testDeserialization_data()
{
    QTest::addColumn<QtJson::Configuration>("config");
    QTest::addColumn<TSerializable>("data");
    QTest::addColumn<QJsonValue>("json");
    QTest::addColumn<QCborValue>("cbor");
    QTest::addColumn<bool>("throws");

    setupData();
    setupDeserData();
}

template<typename TSerializable>
void SerializationTest<TSerializable>::testDeserialization()
{
    QFETCH(QtJson::Configuration, config);
    QFETCH(TSerializable, data);
    QFETCH(QJsonValue, json);
    QFETCH(QCborValue, cbor);
    QFETCH(bool, throws);

    try {
        if (throws) {
            if (!json.isUndefined())
                QVERIFY_EXCEPTION_THROWN(QtJson::SerializableAdapter<TSerializable>::fromJson(json, config), QtJson::Exception);
            if (!cbor.isInvalid())
                QVERIFY_EXCEPTION_THROWN(QtJson::SerializableAdapter<TSerializable>::fromCbor(cbor, config), QtJson::Exception);
        } else {
            if (!json.isUndefined())
                compare(QtJson::SerializableAdapter<TSerializable>::fromJson(json, config), data, __FILE__, __LINE__);
            if (!cbor.isInvalid())
                compare(QtJson::SerializableAdapter<TSerializable>::fromCbor(cbor, config), data, __FILE__, __LINE__);
        }
    } catch (std::exception &e) {
        QFAIL(e.what());
    }
}



template<typename... TSerializables>
void SerializationMultiTest<TSerializables...>::compare(const Variant &actual, const Variant &expected, const char *file, int line) const
{
    if (!QTest::qCompare(actual, expected, "actual", "expected", file, line)){
        std::visit([](const auto &data) {
            qCritical().noquote() << "Actual:  " << data;
        }, actual);
        std::visit([](const auto &data) {
            qCritical().noquote() << "Expected:" << data;
        }, expected);
    }
}

template<typename... TSerializables>
void SerializationMultiTest<TSerializables...>::testSerialization_data()
{
    QTest::addColumn<QtJson::Configuration>("config");
    QTest::addColumn<Variant>("data");
    QTest::addColumn<QJsonValue>("json");
    QTest::addColumn<QCborValue>("cbor");
    QTest::addColumn<bool>("throws");

    setupData();
    setupSerData();
}

template<typename... TSerializables>
void SerializationMultiTest<TSerializables...>::testSerialization()
{
    QFETCH(QtJson::Configuration, config);
    QFETCH(Variant, data);
    QFETCH(QJsonValue, json);
    QFETCH(QCborValue, cbor);
    QFETCH(bool, throws);

    std::visit([this, config, json, cbor, throws](const auto &vData) {
        using TSerializable = std::decay_t<decltype(vData)>;
        try {
            if (throws) {
                if (!json.isUndefined())
                    QVERIFY_EXCEPTION_THROWN(QtJson::SerializableAdapter<TSerializable>::toJson(vData, config), QtJson::Exception);
                if (!cbor.isInvalid())
                    QVERIFY_EXCEPTION_THROWN(QtJson::SerializableAdapter<TSerializable>::toCbor(vData, config), QtJson::Exception);
            } else {
                if (!json.isUndefined())
                    compare(QtJson::SerializableAdapter<TSerializable>::toJson(vData, config), json, __FILE__, __LINE__);
                if (!cbor.isInvalid())
                    compare(QtJson::SerializableAdapter<TSerializable>::toCbor(vData, config), cbor, __FILE__, __LINE__);
            }
        } catch (std::exception &e) {
            QFAIL(e.what());
        }
    }, data);
}

template<typename... TSerializables>
void SerializationMultiTest<TSerializables...>::testDeserialization_data()
{
    QTest::addColumn<QtJson::Configuration>("config");
    QTest::addColumn<Variant>("data");
    QTest::addColumn<QJsonValue>("json");
    QTest::addColumn<QCborValue>("cbor");
    QTest::addColumn<bool>("throws");

    setupData();
    setupDeserData();
}

template<typename... TSerializables>
void SerializationMultiTest<TSerializables...>::testDeserialization()
{
    QFETCH(QtJson::Configuration, config);
    QFETCH(Variant, data);
    QFETCH(QJsonValue, json);
    QFETCH(QCborValue, cbor);
    QFETCH(bool, throws);

    std::visit([this, config, json, cbor, throws](const auto &vData) {
        using TSerializable = std::decay_t<decltype(vData)>;
        try {
            if (throws) {
                if (!json.isUndefined())
                    QVERIFY_EXCEPTION_THROWN(QtJson::SerializableAdapter<TSerializable>::fromJson(json, config), QtJson::Exception);
                if (!cbor.isInvalid())
                    QVERIFY_EXCEPTION_THROWN(QtJson::SerializableAdapter<TSerializable>::fromCbor(cbor, config), QtJson::Exception);
            } else {
                if (!json.isUndefined())
                    compare(QtJson::SerializableAdapter<TSerializable>::fromJson(json, config), vData, __FILE__, __LINE__);
                if (!cbor.isInvalid())
                    compare(QtJson::SerializableAdapter<TSerializable>::fromCbor(cbor, config), vData, __FILE__, __LINE__);
            }
        } catch (std::exception &e) {
            QFAIL(e.what());
        }
    }, data);
}

template<typename... TSerializables>
template<typename TSerializable, typename... TArgs>
typename SerializationMultiTest<TSerializables...>::Variant SerializationMultiTest<TSerializables...>::d(TArgs&&... args) const
{
    return TSerializable{std::forward<TArgs>(args)...};
}
