#include <QtTest>
#include <serializationtest.h>
#include <serializableoptional.h>
#include <testserializable.h>
using namespace QtJson;

template <typename T>
inline QDebug operator<<(QDebug debug, const std::optional<T> &opt) {
	if (opt)
		debug << *opt;
	else
		debug << "null";
	return debug;
}

class OptionalTest : public SerializationMultiTest<
                         std::optional<int>,
                         std::optional<TestSerializable>>
{
	Q_OBJECT

protected:
	void setupData() const override;

private:
    inline Variant di(std::optional<int> &&value = std::nullopt) const {
        return d<std::optional<int>>(std::move(value));
	}

    inline Variant ds(std::optional<double> &&value = std::nullopt, std::optional<QCborKnownTags> &&tag = std::nullopt) const {
        return d<std::optional<TestSerializable>>(value ?
            std::optional<TestSerializable>{{*std::move(value), std::move(tag)}} :
			std::nullopt);
	}
};

Q_DECLARE_METATYPE(OptionalTest::Variant)

void OptionalTest::setupData() const
{
	QTest::addRow("int.null") << Configuration{}
							  << di()
							  << QJsonValue{QJsonValue::Null}
							  << QCborValue{QCborValue::Null}
							  << false;
	QTest::addRow("int.value") << Configuration{}
							   << di(42)
							   << QJsonValue{42}
							   << QCborValue{42}
							   << false;
	QTest::addRow("ser.null") << Configuration{}
							  << ds()
							  << QJsonValue{QJsonValue::Null}
							  << QCborValue{QCborValue::Null}
							  << false;
	QTest::addRow("ser.value") << Configuration{}
							   << ds(4.2)
							   << QJsonValue{4.2}
							   << QCborValue{4.2}
							   << false;
	QTest::addRow("ser.tagged") << Configuration{}
								<< ds(4.2, QCborKnownTags::Decimal)
								<< QJsonValue{QJsonValue::Undefined}
								<< QCborValue{QCborKnownTags::Decimal, 4.2}
								<< false;
}

QTEST_APPLESS_MAIN(OptionalTest)

#include "tst_optional.moc"
