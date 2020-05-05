#pragma once

#include "iserializable.h"
#include "serializablewrapper.h"

#include <type_traits>

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
	Q_INVOKABLE QJsonValue toJson(const QtJson::CommonConfiguration &config = {}) const override;
	Q_INVOKABLE void assignJson(const QJsonValue &value, const QtJson::CommonConfiguration &config = {}) override;
	Q_INVOKABLE QCborValue toCbor(const QtJson::CommonConfiguration &config = {}) const override;
	Q_INVOKABLE void assignCbor(const QCborValue &value, const QtJson::CommonConfiguration &config = {}) override;

protected:
	virtual const QMetaObject *getMetaObject() const = 0;

private:
	static QByteArray serializablePropInfoName(const QMetaProperty &property);
	ISerializable *asSerializable(const QMetaObject *mo,
								  const QMetaProperty &property) const;

	template <typename TValue>
	typename __private::DataValueInfo<TValue>::Map serialize(const CommonConfiguration &config) const;
	template <typename TValue>
	void deserialize(const typename __private::DataValueInfo<TValue>::Map &map, const CommonConfiguration &config);
};

}

#define QTJSON_SERIALIZABLE_KEY "__qtjson_serializable"
#define QTJSON_SERIALIZABLE Q_CLASSINFO(QTJSON_SERIALIZABLE_KEY, "true")

#define QTJSON_SERIALIZABLE_GADGET(Type) \
	QTJSON_SERIALIZABLE \
	public: \
		static Type fromJson(const QJsonValue &value, const QtJson::CommonConfiguration &config = {}) { \
			Type _instance; \
			_instance.assignJson(value, config); \
			return _instance; \
		} \
		\
		static Type fromCbor(const QCborValue &value, const QtJson::CommonConfiguration &config = {}) { \
			Type _instance; \
			_instance.assignCbor(value, config); \
			return _instance; \
		} \
		\
	protected: \
		inline const QMetaObject *getMetaObject() const override { \
			return &Type::staticMetaObject; \
		} \
		\
	private:

#define QTJSON_PROPERTY_OFFSET_KEY "__qtjson_offset"
#define QTJSON_PROPERTY_OFFSET(offset) Q_CLASSINFO(QTJSON_PROPERTY_OFFSET_KEY, #offset)

#define QTJSON_SERIALIZABLE_PROP_KEY_STR "__qtjson_propcast_"
#define QTJSON_SERIALIZABLE_PROP_KEY(name) (QTJSON_SERIALIZABLE_PROP_KEY_STR #name)
#define QTJSON_SERIALIZABLE_PROP(name, member, ...) \
	Q_INVOKABLE inline QtJson::ISerializable *__qtjson_propcast_ ## name() { \
		return QtJson::SerializableCast<__VA_ARGS__>::cast(&(this->member)); \
	}

#define QTJSON_PROP_MEMBER(name, ...) \
	private: \
		QTJSON_SERIALIZABLE_PROP(name, name, typename QtJson::SerializableWrapper<__VA_ARGS__>::type) \
	public: \
		typename QtJson::SerializableWrapper<__VA_ARGS__>::type name

#define QTJSON_PROP(name, ...) \
	Q_PROPERTY(__VA_ARGS__ name MEMBER name STORED true) \
	QTJSON_PROP_MEMBER(name, __VA_ARGS__)
