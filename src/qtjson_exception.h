#pragma once

#include "configuration.h"

#include <optional>

#include <QtCore/QtGlobal>
#include <QtCore/QMetaProperty>
#include <QtCore/QVariant>
#include <QtCore/QJsonValue>
#include <QtCore/QCborValue>
#include <QtCore/QJsonParseError>

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

class QTJSON_EXPORT InvalidValueTypeException : public Exception
{
public:
	InvalidValueTypeException(QJsonValue::Type actual, const QList<QJsonValue::Type> &expected = {});
	InvalidValueTypeException(QCborValue::Type actual, const QList<QCborValue::Type> &expected = {});

	void raise() const override;
	ExceptionBase *clone() const override;

private:
	static QByteArray typeName(QJsonValue::Type type);
	static QByteArray typeName(QCborValue::Type type);

	template <typename T>
	static inline QByteArray join(QList<T> values) {
		QVector<QByteArray> res;
		res.resize(values.size());
		std::transform(values.begin(), values.end(), res.begin(),
					   [](T t) { return typeName(t); });
		return res.toList().join(", ");
	}
};

class QTJSON_EXPORT InvalidValueTagException : public Exception
{
public:
	InvalidValueTagException(QCborTag actual, const QList<QCborTag> &expected);

	void raise() const override;
	ExceptionBase *clone() const override;

private:
	static inline QByteArray join(QList<QCborTag> values) {
		QVector<QByteArray> res;
		res.resize(values.size());
		std::transform(values.begin(), values.end(), res.begin(),
					   [](QCborTag tag) { return QByteArray::number(static_cast<std::underlying_type_t<QCborTag>>(tag)); });
		return res.toList().join(", ");
	}
};

class QTJSON_EXPORT InvalidPropertyValueException : public Exception
{
public:
	InvalidPropertyValueException(const QMetaProperty &property, const QVariant &value);
	InvalidPropertyValueException(const QMetaProperty &property, const QJsonValue &value);
	InvalidPropertyValueException(const QMetaProperty &property, const QCborValue &value);

	void raise() const override;
	ExceptionBase *clone() const override;
};

class QTJSON_EXPORT InvalidPropertyMethodCallException : public Exception
{
public:
    InvalidPropertyMethodCallException(const QMetaProperty &property, const QMetaMethod &method);

    void raise() const override;
    ExceptionBase *clone() const override;
};

class QTJSON_EXPORT ValidationFailureException : public Exception
{
public:
	explicit ValidationFailureException(const QMetaProperty &property);
	explicit ValidationFailureException(const QMetaObject *metaObject, const QStringList &extra);

	void raise() const override;
	ExceptionBase *clone() const override;
};

class QTJSON_EXPORT InvalidDataException : public Exception
{
public:
    explicit InvalidDataException();

    void raise() const override;
    ExceptionBase *clone() const override;
};

class QTJSON_EXPORT JsonParseException : public Exception
{
public:
    explicit JsonParseException(const QJsonParseError &error);

    void raise() const override;
    ExceptionBase *clone() const override;
};

class QTJSON_EXPORT CborParseException : public Exception
{
public:
    explicit CborParseException(const QByteArray &signature);
    explicit CborParseException(const QCborParserError &error);

    void raise() const override;
    ExceptionBase *clone() const override;
};

}
