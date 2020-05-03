#include <QtTest>
#include <serializationtest.h>
#include <serializabledictionary.h>
#include <testserializable.h>
using namespace QtJson;

class DictionaryTest : public SerializationMultiTest<
                           SerializableMap<QString, int>,
                           SerializableHash<QString, int>,
                           SerializableMap<double, bool>,
                           SerializableMap<int, TestSerializable>>
{
    Q_OBJECT

protected:
    void setupData() const override;
    void setupSerData() const override;
    void setupDeserData() const override;

private:
    template <typename... TArgs>
    inline ConstSerPtr dm(TArgs... args) const {
        return d<SerializableMap<QString, int>>(SerializableMap<QString, int>{args...});
    }
    template <typename... TArgs>
    inline ConstSerPtr dh(TArgs... args) const {
        return d<SerializableHash<QString, int>>(SerializableHash<QString, int>{args...});
    }
    template <typename... TArgs>
    inline ConstSerPtr dmk(TArgs... args) const {
        return d<SerializableMap<double, bool>>(SerializableMap<double, bool>{args...});
    }
    template <typename... TArgs>
    inline ConstSerPtr dms(TArgs... args) const {
        return d<SerializableMap<int, TestSerializable>>(SerializableMap<int, TestSerializable>{args...});
    }
};

void DictionaryTest::setupData() const
{
    QTest::addRow("map.filled") << CommonConfiguration{}
                                << dm(std::make_pair(QStringLiteral("a"), 1),
                                      std::make_pair(QStringLiteral("b"), 2),
                                      std::make_pair(QStringLiteral("c"), 3))
                                << QJsonValue{QJsonObject{
                                       {QStringLiteral("a"), 1},
                                       {QStringLiteral("b"), 2},
                                       {QStringLiteral("c"), 3}
                                   }}
                                << QCborValue{QCborMap{
                                       {QStringLiteral("a"), 1},
                                       {QStringLiteral("b"), 2},
                                       {QStringLiteral("c"), 3}
                                   }}
                                << false;
    QTest::addRow("map.empty") << CommonConfiguration{}
                               << dm()
                               << QJsonValue{QJsonObject{}}
                               << QCborValue{QCborMap{}}
                               << false;

    QTest::addRow("hash.empty") << CommonConfiguration{}
                                << dh()
                                << QJsonValue{QJsonObject{}}
                                << QCborValue{QCborMap{}}
                                << false;

    QTest::addRow("map.keyed.filled") << CommonConfiguration{}
                                      << dmk(std::make_pair(1.1, true),
                                             std::make_pair(2.2, false),
                                             std::make_pair(3.3, true))
                                      << QJsonValue{QJsonObject{
                                             {QStringLiteral("1.1"), true},
                                             {QStringLiteral("2.2"), false},
                                             {QStringLiteral("3.3"), true}
                                         }}
                                      << QCborValue{QCborMap{
                                             {1.1, true},
                                             {2.2, false},
                                             {3.3, true}
                                         }}
                                      << false;
    QTest::addRow("map.keyed.empty") << CommonConfiguration{}
                                     << dmk()
                                     << QJsonValue{QJsonObject{}}
                                     << QCborValue{QCborMap{}}
                                     << false;

    QTest::addRow("map.serializable.filled") << CommonConfiguration{}
                                             << dms(std::make_pair(1, TestSerializable{1.1}),
                                                    std::make_pair(2, TestSerializable{2.2}),
                                                    std::make_pair(3, TestSerializable{3.3}))
                                             << QJsonValue{QJsonObject{
                                                    {QStringLiteral("1"), 1.1},
                                                    {QStringLiteral("2"), 2.2},
                                                    {QStringLiteral("3"), 3.3}
                                                }}
                                             << QCborValue{QCborMap{
                                                    {1, 1.1},
                                                    {2, 2.2},
                                                    {3, 3.3}
                                                }}
                                             << false;
    QTest::addRow("map.serializable.empty") << CommonConfiguration{}
                                            << dms()
                                            << QJsonValue{QJsonObject{}}
                                            << QCborValue{QCborMap{}}
                                            << false;
}

void DictionaryTest::setupSerData() const
{
    QTest::addRow("hash.filled.json") << CommonConfiguration{}
                                      << dh(std::make_pair(QStringLiteral("a"), 1),
                                            std::make_pair(QStringLiteral("b"), 2),
                                            std::make_pair(QStringLiteral("c"), 3))
                                      << QJsonValue{QJsonObject{
                                             {QStringLiteral("a"), 1},
                                             {QStringLiteral("b"), 2},
                                             {QStringLiteral("c"), 3}
                                         }}
                                      << QCborValue{QCborValue::Invalid}
                                      << false;
    QTest::addRow("hash.filled.cbor") << CommonConfiguration{}
                                      << dh(std::make_pair(QStringLiteral("a"), 1))
                                      << QJsonValue{QJsonValue::Undefined}
                                      << QCborValue{QCborMap{{QStringLiteral("a"), 1}}}
                                      << false;
}

void DictionaryTest::setupDeserData() const
{
    QTest::addRow("hash.filled") << CommonConfiguration{}
                                 << dh(std::make_pair(QStringLiteral("a"), 1),
                                       std::make_pair(QStringLiteral("b"), 2),
                                       std::make_pair(QStringLiteral("c"), 3))
                                 << QJsonValue{QJsonObject{
                                        {QStringLiteral("a"), 1},
                                        {QStringLiteral("b"), 2},
                                        {QStringLiteral("c"), 3}
                                    }}
                                 << QCborValue{QCborMap{
                                        {QStringLiteral("a"), 1},
                                        {QStringLiteral("b"), 2},
                                        {QStringLiteral("c"), 3}
                                    }}
                                 << false;
}

QTEST_APPLESS_MAIN(DictionaryTest)

#include "tst_dictionary.moc"
