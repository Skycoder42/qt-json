#ifndef QTJSON_EXCEPTION_H
#define QTJSON_EXCEPTION_H

#include <optional>

#include <QtCore/QtGlobal>
#include <QtCore/QMetaProperty>
#include <QtCore/QVariant>

#if !defined(QT_NO_EXCEPTIONS) && QT_CONFIG(future)
#include <QtCore/qexception.h>
namespace QtJson {
using ExceptionBase = QException;
}
#else
#include <exception>
namespace QtJson {
using ExceptionBase = std::exception;
}
#endif

namespace QtJson {

class QTJSON_EXPORT Exception : public ExceptionBase
{
public:
    char const* what() const noexcept final;

    virtual void raise() const;
    virtual ExceptionBase *clone() const;

protected:
    Exception(QByteArray what);

private:
    QByteArray _what;
};

class QTJSON_EXPORT InvalidPropertyValueException : public Exception
{
public:
    InvalidPropertyValueException(const QMetaProperty &property, const QVariant &value);

    void raise() const override;
    ExceptionBase *clone() const override;

private:
};

}

#endif // QTJSON_EXCEPTION_H
