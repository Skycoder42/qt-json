#pragma once

#include "iserializable.h"

#include <QtCore/QByteArray>
#include <QtCore/QDateTime>

namespace QtJson {

template <typename T>
class SerializableWrapperBase : public ISerializable
{
    inline SerializableWrapperBase(void *value) :
        _value{reinterpret_cast<T*>(value)}
	{}

protected:
	inline const T &value() const {
		return *_value;
	}
	void setValue(const T &value) {
		*_value = value;
	}

private:
	T *_value;
};

template <typename T>
class SerializableWrapper;

template <>
class QTJSON_EXPORT SerializableWrapper<QByteArray> : public SerializableWrapperBase<QByteArray>
{
public:
	QJsonValue toJson(const JsonConfiguration &config) const override;
	void assignJson(const QJsonValue &value, const JsonConfiguration &config) override;
	QCborValue toCbor(const CborConfiguration &config) const override;
	void assignCbor(const QCborValue &value, const CborConfiguration &config) override;
};

template <>
class QTJSON_EXPORT SerializableWrapper<QDateTime> : public SerializableWrapperBase<QDateTime>
{
public:
    QJsonValue toJson(const JsonConfiguration &config) const override;
    void assignJson(const QJsonValue &value, const JsonConfiguration &config) override;
    QCborValue toCbor(const CborConfiguration &config) const override;
    void assignCbor(const QCborValue &value, const CborConfiguration &config) override;
};

}
