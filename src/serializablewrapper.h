#pragma once

#include "iserializable.h"
#include "iserializablewrapperfactory.h"

#include <QtCore/QByteArray>
#include <QtCore/QDateTime>

namespace QtJson {

template <typename T>
class SerializableWrapper;

template <typename T>
class SerializableWrapperBase : public ISerializable
{
public:
    class Factory : public ITypedSerializableWrapperFactory<T> {
    public:
        inline ISerializable *createWrapper(T *data) const override {
            return new SerializableWrapper<T>(data);
        }
    };

    inline SerializableWrapperBase(T *value) :
        _value{value}
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

template <>
class QTJSON_EXPORT SerializableWrapper<QByteArray> : public SerializableWrapperBase<QByteArray>
{
public:
    using SerializableWrapperBase::SerializableWrapperBase;

	QJsonValue toJson(const JsonConfiguration &config) const override;
	void assignJson(const QJsonValue &value, const JsonConfiguration &config) override;
	QCborValue toCbor(const CborConfiguration &config) const override;
	void assignCbor(const QCborValue &value, const CborConfiguration &config) override;
};

template <>
class QTJSON_EXPORT SerializableWrapper<QDateTime> : public SerializableWrapperBase<QDateTime>
{
public:
    using SerializableWrapperBase::SerializableWrapperBase;

    QJsonValue toJson(const JsonConfiguration &config) const override;
    void assignJson(const QJsonValue &value, const JsonConfiguration &config) override;
    QCborValue toCbor(const CborConfiguration &config) const override;
    void assignCbor(const QCborValue &value, const CborConfiguration &config) override;
};

// TODO add NetworkAddress(Prefix) 260/261
// TODO add Regular expression / mime / uuid?

}
