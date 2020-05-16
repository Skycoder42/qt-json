#include <QtTest>
#include <serializationtest.h>
#include "testgadget.h"
using namespace QtJson;

class GadgetTest : public SerializationTest<TestGadget>
{
	Q_OBJECT

protected:
	void setupData() const override;
	void setupSerData() const override;
	void setupDeserData() const override;

private:
	inline Configuration ce(bool enumAsString) const {
		Configuration config;
		config.enumAsString = enumAsString;
		return config;
	}

	inline Configuration cs(bool ignoreStored) const {
		Configuration config;
		config.ignoreStored = ignoreStored;
		return config;
	}

	inline Configuration cv(ValidationFlags flags) const {
		Configuration config;
		config.validation = flags;
		return config;
	}

    inline TestGadget dx(double data) const {
		TestGadget gadget;
		gadget.prop6.value = data;
		return d(gadget);
	}

    inline TestGadget de(TestGadget::Flags f, TestGadget::Enum e) const {
		TestGadget gadget;
		gadget.prop7 = f;
		gadget.prop8 = e;
		return d(gadget);
	}

    inline TestGadget ds(int prop2) const {
		TestGadget gadget;
		gadget.prop2 = prop2;
		return d(gadget);
	}

	inline QJsonValue gj(const QJsonObject &value = {}, const QStringList &rmKeys = {}) const {
		QJsonObject jObj{
			{QStringLiteral("prop1"), 1},
			{QStringLiteral("prop3"), 3},
			{QStringLiteral("prop4"), 4},
            {QStringLiteral("prop5"), 5.5},
			{QStringLiteral("prop6"), 0.0},
			{QStringLiteral("prop7"), QString{}},
			{QStringLiteral("prop8"), QStringLiteral("Value1")},
		};

		for (auto it = value.begin(), end = value.end(); it != end; ++it)
			jObj.insert(it.key(), it.value());

		for (const auto &key : rmKeys)
			jObj.remove(key);

		return jObj;
	}

	inline QCborValue gc(const QCborMap &value = {}, const QStringList &rmKeys = {}) const {
		QCborMap cMap{
			{QStringLiteral("prop1"), 1},
			{QStringLiteral("prop3"), 3},
			{QStringLiteral("prop4"), 4},
            {QStringLiteral("prop5"), 5.5},
			{QStringLiteral("prop6"), 0.0},
			{QStringLiteral("prop7"), QString{}},
			{QStringLiteral("prop8"), QStringLiteral("Value1")},
		};

		for (const auto &val : value)
			cMap.insert(val.first, val.second);

		for (const auto &key : rmKeys)
			cMap.remove(key);

		return cMap;
	}
};

void GadgetTest::setupData() const
{
	QTest::addRow("default") << Configuration{}
							 << d()
							 << gj()
							 << gc()
							 << false;

	TestGadget filled;
	filled.prop1 = 11;
	filled.prop3 = 33;
	filled.prop4 = 44;
	filled.prop5 = 5.5;
	filled.prop6.value = 6.6;
	filled.prop7 = TestGadget::Flag::Flag1 | TestGadget::Flag::Flag2;
	filled.prop8 = TestGadget::Enum::Value3;
	QTest::addRow("filled") << Configuration{}
							<< d(filled)
							<< QJsonValue{QJsonObject{
								   {QStringLiteral("prop1"), 11},
								   {QStringLiteral("prop3"), 33},
								   {QStringLiteral("prop4"), 44},
								   {QStringLiteral("prop5"), 5.5},
								   {QStringLiteral("prop6"), 6.6},
								   {QStringLiteral("prop7"), QStringLiteral("Flag1|Flag2")},
								   {QStringLiteral("prop8"), QStringLiteral("Value3")},
							   }}
							<< QCborValue{QCborMap{
								   {QStringLiteral("prop1"), 11},
								   {QStringLiteral("prop3"), 33},
								   {QStringLiteral("prop4"), 44},
								   {QStringLiteral("prop5"), 5.5},
								   {QStringLiteral("prop6"), 6.6},
								   {QStringLiteral("prop7"), QStringLiteral("Flag1|Flag2")},
								   {QStringLiteral("prop8"), QStringLiteral("Value3")},
							   }}
							<< false;

	QTest::addRow("enums") << ce(false)
						   << de(TestGadget::Flag::Flag5, TestGadget::Enum::Value0)
						   << gj({
								   {QStringLiteral("prop7"), 5},
								   {QStringLiteral("prop8"), 0}
							  })
						   << gc({
								  {QStringLiteral("prop7"), 5},
								  {QStringLiteral("prop8"), 0}
							  })
						   << false;
}

void GadgetTest::setupSerData() const
{
	QTest::addRow("stored") << cs(true)
							<< ds(42)
							<< gj({{QStringLiteral("prop2"), 42}})
							<< QCborValue{QCborValue::Invalid}
							<< false;
}

void GadgetTest::setupDeserData() const
{
	QTest::addRow("stored") << cs(true)
							<< ds(42)
							<< gj({{QStringLiteral("prop2"), 42}})
							<< gc({{QStringLiteral("prop2"), 42}})
							<< false;

	QTest::addRow("validate.missing.allowed") << cv(ValidationFlag::None)
											  << d()
											  << gj({}, {QStringLiteral("prop1")})
											  << gc({}, {QStringLiteral("prop1")})
											  << false;
	QTest::addRow("validate.missing.forbidden") << cv(ValidationFlag::NoMissing)
												<< d()
												<< gj({}, {QStringLiteral("prop1")})
												<< gc({}, {QStringLiteral("prop1")})
												<< true;
	QTest::addRow("validate.extra.allowed") << cv(ValidationFlag::None)
											<< d()
											<< gj({{QStringLiteral("prop2"), 32}})
											<< gc({{QStringLiteral("prop2"), 32}})
											<< false;
	QTest::addRow("validate.extra.forbidden") << cv(ValidationFlag::NoExtra)
											  << d()
											  << gj({{QStringLiteral("prop2"), 32}})
											  << gc({{QStringLiteral("prop2"), 32}})
											  << true;

	QTest::addRow("invalid.data") << Configuration{}
								  << d()
								  << QJsonValue{42}
								  << QCborValue{42}
								  << true;
	QTest::addRow("invalid.enum.type") << Configuration{}
									   << d()
									   << gj({
											  {QStringLiteral("prop7"), true}
										  })
									   << gc({
											  {QStringLiteral("prop7"), true}
										  })
									   << true;
	QTest::addRow("invalid.enum.strng") << Configuration{}
										<< d()
										<< gj({
											   {QStringLiteral("prop7"), QStringLiteral("invalid")}
										   })
										<< gc({
											   {QStringLiteral("prop7"), QStringLiteral("invalid")}
										   })
										<< true;
	QTest::addRow("invalid.value") << Configuration{}
								   << d()
								   << gj({
										  {QStringLiteral("prop1"), QStringLiteral("test")}
									  })
								   << gc({
										  {QStringLiteral("prop1"), QStringLiteral("test")}
									  })
								   << true;
}

QTEST_APPLESS_MAIN(GadgetTest)

#include "tst_gadget.moc"
