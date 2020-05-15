#include "qtjson_exception.h"
using namespace QtJson;

const char *Exception::what() const noexcept
{
	return _what.constData();
}

void Exception::raise() const
{
	throw *this;
}

ExceptionBase *Exception::clone() const
{
	return new Exception{*this};
}

Exception::Exception(QByteArray what) :
	_what{std::move(what)}
{}



InvalidValueTypeException::InvalidValueTypeException(QJsonValue::Type actual, const QList<QJsonValue::Type> &expected) :
	Exception{
		"Unabled to deserialize JSON value of type " +
		typeName(actual) +
		(expected.isEmpty() ?
			QByteArray{} :
			" - expected type to be one of: " + join(expected))
	}
{}

InvalidValueTypeException::InvalidValueTypeException(QCborValue::Type actual, const QList<QCborValue::Type> &expected) :
	Exception{
		"Unabled to deserialize CBOR value of type " +
		typeName(actual) +
		(expected.isEmpty() ?
			QByteArray{} :
			" - expected type to be one of: " + join(expected))
	}
{}

void InvalidValueTypeException::raise() const
{
	throw *this;
}

ExceptionBase *InvalidValueTypeException::clone() const
{
	return new InvalidValueTypeException{*this};
}

QByteArray InvalidValueTypeException::typeName(QJsonValue::Type type)
{
	switch (type) {
	case QJsonValue::Null:
		return "null";
	case QJsonValue::Bool:
		return "bool";
	case QJsonValue::Double:
		return "double";
	case QJsonValue::String:
		return "string";
	case QJsonValue::Array:
		return "array";
	case QJsonValue::Object:
		return "object";
	case QJsonValue::Undefined:
		return "undefined";
	default:
		Q_UNREACHABLE();
	}
}

QByteArray InvalidValueTypeException::typeName(QCborValue::Type type)
{
	switch (type) {
	case QCborValue::Integer:
		return "integer";
	case QCborValue::ByteArray:
		return "bytearray";
	case QCborValue::String:
		return "string";
	case QCborValue::Array:
		return "array";
	case QCborValue::Map:
		return "map";
	case QCborValue::Tag:
		return "tag";
	case QCborValue::SimpleType:
		return "simple";
	case QCborValue::False:
		return "false";
	case QCborValue::True:
		return "true";
	case QCborValue::Null:
		return "null";
	case QCborValue::Undefined:
		return "undefined";
	case QCborValue::Double:
		return "double";
	case QCborValue::DateTime:
		return "datetime";
	case QCborValue::Url:
		return "url";
	case QCborValue::RegularExpression:
		return "regexp";
	case QCborValue::Uuid:
		return "uuid";
	case QCborValue::Invalid:
		return "invalid";
	default:
		Q_UNREACHABLE();
	}
}



InvalidValueTagException::InvalidValueTagException(QCborTag actual, const QList<QCborTag> &expected) :
	Exception{
		"Unexpected CBOR-Tag " +
		QByteArray::number(static_cast<std::underlying_type_t<QCborTag>>(actual)) +
		(expected.isEmpty() ?
							QByteArray{} :
							" - expected tag to be one of: " + join(expected))
	}
{}

void InvalidValueTagException::raise() const
{
	throw *this;
}

ExceptionBase *InvalidValueTagException::clone() const
{
	return new InvalidValueTagException{*this};
}




InvalidPropertyValueException::InvalidPropertyValueException(const QMetaProperty &property, const QVariant &value) :
	Exception{
		QByteArrayLiteral("Unabled to set property ") +
		property.name() +
		" off " +
		property.enclosingMetaObject()->className() +
		": Variant of type \"" +
		value.typeName() +
		"\" cannot be convert to \"" +
		property.typeName() +
		"\""
	}
{}

InvalidPropertyValueException::InvalidPropertyValueException(const QMetaProperty &property, const QJsonValue &value) :
	Exception{
		QByteArrayLiteral("Unabled to set property ") +
		property.name() +
		" off " +
		property.enclosingMetaObject()->className() +
		": Variant of type " +
		QByteArray::number(value.type()) +
		" cannot be convert to \"" +
		property.typeName() +
		"\""
	}
{}

InvalidPropertyValueException::InvalidPropertyValueException(const QMetaProperty &property, const QCborValue &value) :
	Exception{
		QByteArrayLiteral("Unabled to set property ") +
		property.name() +
		" of " +
		property.enclosingMetaObject()->className() +
		": Variant of type " +
		QByteArray::number(value.type()) +
		(value.isTag() ?
					   " (Tag: " + QByteArray::number(static_cast<quint64>(value.tag())) + ")" :
					   QByteArray{}) +
		" cannot be convert to \"" +
		property.typeName() +
		"\""
	}
{}

void InvalidPropertyValueException::raise() const
{
	throw *this;
}

ExceptionBase *InvalidPropertyValueException::clone() const
{
	return new InvalidPropertyValueException{*this};
}



InvalidPropertyMethodCallException::InvalidPropertyMethodCallException(const QMetaProperty &property, const QMetaMethod &method) :
    Exception{
        QByteArrayLiteral("Failed to invoke meta method \"") +
        method.methodSignature() +
        "\" for property " +
        property.name() +
        " on gadget of type " +
        property.enclosingMetaObject()->className()
    }
{}

void InvalidPropertyMethodCallException::raise() const
{
    throw *this;
}

ExceptionBase *InvalidPropertyMethodCallException::clone() const
{
    return new InvalidPropertyMethodCallException{*this};
}



ValidationFailureException::ValidationFailureException(const QMetaProperty &property) :
	Exception{
		QByteArrayLiteral("Property ") +
		property.name() +
		" of " +
		property.enclosingMetaObject()->className() +
		" is missing in given JSON/CBOR map"
	}
{}

ValidationFailureException::ValidationFailureException(const QMetaObject *metaObject, const QStringList &extra) :
	Exception {
		QByteArrayLiteral("Found ") +
		QByteArray::number(extra.size()) +
		" additional values in given JSON/CBOR, but they are not declared for " +
		metaObject->className() +
		" - the keys of these values are: " +
		extra.join(QStringLiteral(", ")).toUtf8()
	}
{}

void ValidationFailureException::raise() const
{
	throw *this;
}

ExceptionBase *ValidationFailureException::clone() const
{
	return new ValidationFailureException{*this};
}



InvalidDataException::InvalidDataException() :
    Exception{"Got unexpected invalid CBOR/JSON data! Unable to continue serialization"}
{}

void InvalidDataException::raise() const
{
    throw *this;
}

ExceptionBase *InvalidDataException::clone() const
{
    return new InvalidDataException{*this};
}



JsonParseException::JsonParseException(const QJsonParseError &error) :
    Exception{
        "Failed to parse JSON data at position " +
        QByteArray::number(error.offset) +
        " with error: " +
        error.errorString().toUtf8()
    }
{}

void JsonParseException::raise() const
{
    throw *this;
}

ExceptionBase *JsonParseException::clone() const
{
    return new JsonParseException{*this};
}



CborParseException::CborParseException(const QByteArray &signature) :
    Exception{
        "Unable to parse CBOR data - expected signature to be 0xd9d9f7, but was 0x" +
        signature.toHex().toLower()
    }
{}

CborParseException::CborParseException(const QCborParserError &error) :
    Exception{
        "Failed to parse CBOR data at position " +
        QByteArray::number(error.offset) +
        " with error: " +
        error.errorString().toUtf8()
    }
{}

void CborParseException::raise() const
{
    throw *this;
}

ExceptionBase *CborParseException::clone() const
{
    return new CborParseException{*this};
}
