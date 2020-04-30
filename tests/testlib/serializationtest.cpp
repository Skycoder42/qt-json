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
    const auto lStr = lhs.toDiagnosticNotation(QCborValue::Compact).toUtf8();
    const auto rStr = rhs.toDiagnosticNotation(QCborValue::Compact).toUtf8();
    if (!QTest::qCompare(lhs, rhs, lStr.constData(), rStr.constData(), file, line)){
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
