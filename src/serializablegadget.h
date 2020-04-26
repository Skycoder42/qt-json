#pragma once

#include "iserializable.h"
#include "qtjson_metamacros.h"

#include <QtCore/QObject>

namespace QtJson {

namespace __private {

template <typename TType>
struct DataValueInfo;

}

class QTJSON_EXPORT SerializableGadget : public ISerializable
{
	Q_GADGET

public:
	Q_INVOKABLE QJsonValue toJson(const JsonConfiguration &config = {}) const override;
	Q_INVOKABLE void assignJson(const QJsonValue &value, const JsonConfiguration &config = {}) override;
	Q_INVOKABLE QCborValue toCbor(const CborConfiguration &config = {}) const override;
	Q_INVOKABLE void assignCbor(const QCborValue &value, const CborConfiguration &config = {}) override;

protected:
	virtual const QMetaObject *getMetaObject() const = 0;

private:
	static QByteArray serializablePropInfoName(const QMetaProperty &property);
	ISerializable *asSerializable(const QMetaObject *mo,
								  const QMetaProperty &property,
								  QVariant &variant) const;

	template <typename TValue>
	typename __private::DataValueInfo<TValue>::Map serialize(const typename __private::DataValueInfo<TValue>::Config &config) const;
	template <typename TValue>
	void deserialize(const typename __private::DataValueInfo<TValue>::Map &map, const typename __private::DataValueInfo<TValue>::Config &config);
};

}

#define QTJSON_SERIALIZABLE_GADGET(Type) \
	QTJSON_SERIALIZABLE \
	public: \
		static Type fromJson(const QJsonValue &value) { \
			Type _instance; \
			_instance.assignJson(value); \
			return _instance; \
		} \
		\
		static Type fromCbor(const QCborValue &value) { \
			Type _instance; \
			_instance.assignCbor(value); \
			return _instance; \
		} \
		\
	protected: \
		inline const QMetaObject *getMetaObject() const override { \
			return &Type::staticMetaObject; \
		} \
		\
	private:
