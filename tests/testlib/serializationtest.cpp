#include "serializationtest.h"
#include <QtCore/QJsonDocument>
#include <QtCore/QDebug>
using namespace QtJson;

void SerializationTestBase::setupData() const {}

void SerializationTestBase::setupSerData() const {}

void SerializationTestBase::setupDeserData() const {}

void SerializationTestBase::compare(const QJsonValue &actual, const QJsonValue &expected, const char *file, int line) const
{
    if (!QTest::qCompare(actual, expected, "actual", "expected", file, line)) {
        qCritical().noquote() << "Actual:  " << stringify(actual);
        qCritical().noquote() << "Expected:" << stringify(expected);
	}
}

void SerializationTestBase::compare(const QCborValue &actual, const QCborValue &expected, const char *file, int line) const
{
    if (!QTest::qCompare(actual, expected, "actual", "expected", file, line)){
        qCritical().noquote() << "Actual:  " << actual.toDiagnosticNotation(QCborValue::Compact);
        qCritical().noquote() << "Expected:" << expected.toDiagnosticNotation(QCborValue::Compact);
	}
}

QByteArray SerializationTestBase::stringify(const QJsonValue &value) const
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
	QTest::addColumn<QtJson::Configuration>("config");
	QTest::addColumn<ConstSerPtr>("data");
	QTest::addColumn<QJsonValue>("json");
	QTest::addColumn<QCborValue>("cbor");
	QTest::addColumn<bool>("throws");

	setupData();
	setupSerData();
}

void SerializationTestBase::testSerialization()
{
	QFETCH(QtJson::Configuration, config);
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
	QTest::addColumn<QtJson::Configuration>("config");
	QTest::addColumn<ConstSerPtr>("data");
	QTest::addColumn<QJsonValue>("json");
	QTest::addColumn<QCborValue>("cbor");
	QTest::addColumn<bool>("throws");

	setupData();
	setupDeserData();
}

void SerializationTestBase::testDeserialization()
{
	QFETCH(QtJson::Configuration, config);
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
