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
        " off " +
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
