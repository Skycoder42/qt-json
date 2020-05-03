#include "serializationtest.h"
#include <QtCore/QJsonDocument>
#include <QtCore/QDebug>
using namespace QtJson;

void SerializationTestBase::setupData() const {}

void SerializationTestBase::setupSerData() const {}

void SerializationTestBase::setupDeserData() const {}

void SerializationTestBase::compare(const QJsonValue &lhs, const QJsonValue &rhs, const char *file, int line) const
{
	const auto lStr = stringiy(lhs);
	const auto rStr = stringiy(rhs);
	if (!QTest::qCompare(lhs, rhs, lStr.constData(), rStr.constData(), file, line)){
		qCritical().noquote() << "Actual:  " << lStr;
		qCritical().noquote() << "Expected:" << rStr;
	}
}

void SerializationTestBase::compare(const QCborValue &lhs, const QCborValue &rhs, const char *file, int line) const
{
	const auto lStr = lhs.toDiagnosticNotation(QCborValue::Compact);
	const auto rStr = rhs.toDiagnosticNotation(QCborValue::Compact);
	if (!QTest::qCompare(lhs, rhs, qUtf8Printable(lStr), qUtf8Printable(rStr), file, line)){
		qCritical().noquote() << "Actual:  " << lStr;
		qCritical().noquote() << "Expected:" << rStr;
	}
}

QByteArray SerializationTestBase::stringiy(const QJsonValue &value) const
{
	switch (value.type()) {
	case QJsonValue::Array:
		return QJsonDocument{value.toArray()}.toJson(QJsonDocument::Compact);
	case QJsonValue::Object:
		return QJsonDocument{value.toObject()}.toJson(QJsonDocument::Compact);
	default:
		return value.toVariant().toString().toUtf8();
	}
}

void SerializationTestBase::testSerialization_data()
{
	QTest::addColumn<QtJson::CommonConfiguration>("config");
	QTest::addColumn<ConstSerPtr>("data");
	QTest::addColumn<QJsonValue>("json");
	QTest::addColumn<QCborValue>("cbor");
	QTest::addColumn<bool>("throws");

	setupData();
	setupSerData();
}

void SerializationTestBase::testSerialization()
{
	QFETCH(QtJson::CommonConfiguration, config);
	QFETCH(ConstSerPtr, data);
	QFETCH(QJsonValue, json);
	QFETCH(QCborValue, cbor);
	QFETCH(bool, throws);

	try {
		if (throws) {
			if (!json.isUndefined())
				QVERIFY_EXCEPTION_THROWN(data->toJson(config), QtJson::Exception);
			if (!cbor.isInvalid())
				QVERIFY_EXCEPTION_THROWN(data->toCbor(config), QtJson::Exception);
		} else {
			if (!json.isUndefined())
				compare(data->toJson(config), json, __FILE__, __LINE__);
			if (!cbor.isInvalid())
				compare(data->toCbor(config), cbor, __FILE__, __LINE__);
		}
	} catch (std::exception &e) {
		QFAIL(e.what());
	}
}

void SerializationTestBase::testDeserialization_data()
{
	QTest::addColumn<QtJson::CommonConfiguration>("config");
	QTest::addColumn<ConstSerPtr>("data");
	QTest::addColumn<QJsonValue>("json");
	QTest::addColumn<QCborValue>("cbor");
	QTest::addColumn<bool>("throws");

	setupData();
	setupDeserData();
}

void SerializationTestBase::testDeserialization()
{
	QFETCH(QtJson::CommonConfiguration, config);
	QFETCH(ConstSerPtr, data);
	QFETCH(QJsonValue, json);
	QFETCH(QCborValue, cbor);
	QFETCH(bool, throws);

	try {
		const auto jData = cloneEmpty(data);
		QVERIFY(jData);
		const auto cData = cloneEmpty(data);
		QVERIFY(cData);

		if (throws) {
			if (!json.isUndefined())
				QVERIFY_EXCEPTION_THROWN(jData->assignJson(json, config), QtJson::Exception);
			if (!cbor.isInvalid())
				QVERIFY_EXCEPTION_THROWN(cData->assignCbor(cbor, config), QtJson::Exception);
		} else {
			if (!json.isUndefined()){
				jData->assignJson(json, config);
				compare(jData, data, __FILE__, __LINE__);
			}
			if (!cbor.isInvalid()) {
				cData->assignCbor(cbor, config);
				compare(cData, data, __FILE__, __LINE__);
			}
		}
	} catch (std::exception &e) {
		QFAIL(e.what());
	}
}
