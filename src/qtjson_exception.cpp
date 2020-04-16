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

void InvalidPropertyValueException::raise() const
{
    throw *this;
}

ExceptionBase *InvalidPropertyValueException::clone() const
{
    return new InvalidPropertyValueException{*this};
}
