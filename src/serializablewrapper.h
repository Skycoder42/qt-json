#pragma once

#include "iserializable.h"

#include <QtCore/QByteArray>

namespace QtJson {

template <typename T>
class SerializableWrapper : public ISerializable
{
	inline SerializableWrapper() :
		_value{new T{}}  // TODO
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

class QTJSON_EXPORT QByteArraySerializableWrapper : public SerializableWrapper<QByteArray>
{
public:
	QJsonValue toJson(const JsonConfiguration &config) const override;
	void assignJson(const QJsonValue &value, const JsonConfiguration &config) override;
	QCborValue toCbor(const CborConfiguration &config) const override;
	void assignCbor(const QCborValue &value, const CborConfiguration &config) override;
};

}
