#include "qtjson.h"

QString QtJson::writeJson(const QJsonValue &value, QJsonDocument::JsonFormat format)
{
    switch (value.type()) {
    case QJsonValue::Undefined:
        throw InvalidDataException{};
    case QJsonValue::Null:
    case QJsonValue::Bool:
    case QJsonValue::Double:
    case QJsonValue::String: {
        const auto data = QJsonDocument{QJsonArray{value}}.toJson(QJsonDocument::Compact);
        return QString::fromUtf8(data.mid(1, data.size() - 2));
    }
    case QJsonValue::Array:
        return QString::fromUtf8(QJsonDocument{value.toArray()}.toJson(format));
    case QJsonValue::Object:
        return QString::fromUtf8(QJsonDocument{value.toObject()}.toJson(format));
    default:
        Q_UNREACHABLE();
    }
}

QJsonValue QtJson::readJson(const QString &data)
{
    QJsonParseError error;
    const auto doc = QJsonDocument::fromJson(data.toUtf8(), &error);
    if (error.error == QJsonParseError::IllegalValue) {
        QJsonParseError subError;
        const auto subDoc = QJsonDocument::fromJson("[" + data.toUtf8() + "]", &subError);
        if (error.error != QJsonParseError::NoError)
            throw JsonParseException{error};
        else
            return subDoc.array().first();
    } else if (error.error != QJsonParseError::NoError) {
        throw JsonParseException{error};
    } else {
        if (doc.isObject())
            return doc.object();
        else if (doc.isArray())
            return doc.array();
        else if (doc.isNull())
            return QJsonValue::Null;
        else
            throw InvalidDataException{};
    }
}

QByteArray QtJson::writeCbor(const QCborValue &value, QCborValue::EncodingOptions opt)
{
    if (value.isInvalid())
        throw InvalidDataException{};
    return QCborValue{QCborKnownTags::Signature, value}.toCbor(opt);
}

QCborValue QtJson::readCbor(const QByteArray &value, bool requireSignature)
{
    if (requireSignature &&
        value.mid(0, 3) != __private::CborSignature) {
        throw CborParseException{value.mid(0, 3)};
    }

    QCborParserError error;
    const QCborValue cbor = QCborValue::fromCbor(value, &error);
    if (error.error != QCborError::NoError)
        throw CborParseException{error};
    else {
        if (cbor.isTag() && cbor.tag() == QCborKnownTags::Signature)
            return cbor.taggedValue();
        else
            return cbor;
    }
}
