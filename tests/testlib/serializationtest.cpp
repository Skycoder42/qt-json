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
