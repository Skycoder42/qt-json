#include <QtTest>
#include <serializationtest.h>
#include <serializabledictionary.h>
#include <testserializable.h>
using namespace QtJson;

class DictionaryTest : public SerializationMultiTest<
                           QMap<QString, int>,
                           QHash<QString, int>,
                           QMap<double, bool>,
                           QMap<int, TestSerializable>>
{
	Q_OBJECT

protected:
	void setupData() const override;
	void setupSerData() const override;
	void setupDeserData() const override;

private:
	template <typename... TArgs>
    inline Variant dm(TArgs&&... args) const {
        return d<QMap<QString, int>>(std::forward<TArgs>(args)...);
	}
	template <typename... TArgs>
    inline Variant dh(TArgs&&... args) const {
        return d<QHash<QString, int>>(std::forward<TArgs>(args)...);
	}
	template <typename... TArgs>
    inline Variant dmk(TArgs&&... args) const {
        return d<QMap<double, bool>>(std::forward<TArgs>(args)...);
	}
	template <typename... TArgs>
    inline Variant dms(TArgs&&... args) const {
        return d<QMap<int, TestSerializable>>(std::forward<TArgs>(args)...);
	}
};

Q_DECLARE_METATYPE(DictionaryTest::Variant)

void DictionaryTest::setupData() const
{
	QTest::addRow("map.filled") << Configuration{}
								<< dm(std::make_pair(QStringLiteral("a"), 1),
									  std::make_pair(QStringLiteral("b"), 2),
									  std::make_pair(QStringLiteral("c"), 3))
								<< QJsonValue{QJsonObject{
									   {QStringLiteral("a"), 1},
									   {QStringLiteral("b"), 2},
									   {QStringLiteral("c"), 3}
								   }}
								<< QCborValue{MapTypeTag, QCborMap{
									   {QStringLiteral("a"), 1},
									   {QStringLiteral("b"), 2},
									   {QStringLiteral("c"), 3}
								   }}
								<< false;
	QTest::addRow("map.empty") << Configuration{}
							   << dm()
							   << QJsonValue{QJsonObject{}}
							   << QCborValue{MapTypeTag, QCborMap{}}
							   << false;

	QTest::addRow("hash.empty") << Configuration{}
								<< dh()
								<< QJsonValue{QJsonObject{}}
								<< QCborValue{MapTypeTag, QCborMap{}}
								<< false;

	QTest::addRow("map.keyed.filled") << Configuration{}
									  << dmk(std::make_pair(1.1, true),
											 std::make_pair(2.2, false),
											 std::make_pair(3.3, true))
									  << QJsonValue{QJsonObject{
											 {QStringLiteral("1.1"), true},
											 {QStringLiteral("2.2"), false},
											 {QStringLiteral("3.3"), true}
										 }}
									  << QCborValue{MapTypeTag, QCborMap{
											 {1.1, true},
											 {2.2, false},
											 {3.3, true}
										 }}
									  << false;
	QTest::addRow("map.keyed.empty") << Configuration{}
									 << dmk()
									 << QJsonValue{QJsonObject{}}
									 << QCborValue{MapTypeTag, QCborMap{}}
									 << false;

	QTest::addRow("map.serializable.filled") << Configuration{}
											 << dms(std::make_pair(1, TestSerializable{1.1}),
													std::make_pair(2, TestSerializable{2.2}),
													std::make_pair(3, TestSerializable{3.3}))
											 << QJsonValue{QJsonObject{
													{QStringLiteral("1"), 1.1},
													{QStringLiteral("2"), 2.2},
													{QStringLiteral("3"), 3.3}
												}}
											 << QCborValue{MapTypeTag, QCborMap{
													{1, 1.1},
													{2, 2.2},
													{3, 3.3}
												}}
											 << false;
	QTest::addRow("map.serializable.empty") << Configuration{}
											<< dms()
											<< QJsonValue{QJsonObject{}}
											<< QCborValue{MapTypeTag, QCborMap{}}
											<< false;
}

void DictionaryTest::setupSerData() const
{
	QTest::addRow("hash.filled.json") << Configuration{}
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
	QTest::addRow("hash.filled.cbor") << Configuration{}
									  << dh(std::make_pair(QStringLiteral("a"), 1))
									  << QJsonValue{QJsonValue::Undefined}
									  << QCborValue{MapTypeTag, QCborMap{{QStringLiteral("a"), 1}}}
									  << false;
}

void DictionaryTest::setupDeserData() const
{
	QTest::addRow("hash.filled") << Configuration{}
								 << dh(std::make_pair(QStringLiteral("a"), 1),
									   std::make_pair(QStringLiteral("b"), 2),
									   std::make_pair(QStringLiteral("c"), 3))
								 << QJsonValue{QJsonObject{
										{QStringLiteral("a"), 1},
										{QStringLiteral("b"), 2},
										{QStringLiteral("c"), 3}
									}}
								 << QCborValue{MapTypeTag, QCborMap{
										{QStringLiteral("a"), 1},
										{QStringLiteral("b"), 2},
										{QStringLiteral("c"), 3}
									}}
								 << false;

	QTest::addRow("map.untagged") << Configuration{}
								  << dm(std::make_pair(QStringLiteral("a"), 1),
										std::make_pair(QStringLiteral("b"), 2),
										std::make_pair(QStringLiteral("c"), 3))
								  << QJsonValue{QJsonValue::Undefined}
								  << QCborValue{QCborMap{
										 {QStringLiteral("a"), 1},
										 {QStringLiteral("b"), 2},
										 {QStringLiteral("c"), 3}
									 }}
								  << false;

	QTest::addRow("hash.untagged") << Configuration{}
								   << dh(std::make_pair(QStringLiteral("a"), 1),
										 std::make_pair(QStringLiteral("b"), 2),
										 std::make_pair(QStringLiteral("c"), 3))
								   << QJsonValue{QJsonValue::Undefined}
								   << QCborValue{QCborMap{
										  {QStringLiteral("a"), 1},
                                          {QStringLiteral("b"), 2},
										  {QStringLiteral("c"), 3}
									  }}
								   << false;

	QTest::addRow("invalid") << Configuration{}
							 << dm()
							 << QJsonValue{42}
							 << QCborValue{42}
							 << true;
}

QTEST_APPLESS_MAIN(DictionaryTest)

#include "tst_dictionary.moc"
