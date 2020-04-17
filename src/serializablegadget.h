#ifndef QTJSON_SERIALIZABLEGADGET_H
#define QTJSON_SERIALIZABLEGADGET_H

#include "iserializable.h"
#include "qtjson_metamacros.h"

#include <QtCore/QObject>

namespace QtJson {

class QTJSON_EXPORT SerializableGadget : public ISerializable
{
    Q_GADGET

public:
    Q_INVOKABLE QJsonValue toJson() const override;
    Q_INVOKABLE void assignJson(const QJsonValue &value) override;
    Q_INVOKABLE QCborValue toCbor() const override;
    Q_INVOKABLE void assignCbor(const QCborValue &value) override;

protected:
    virtual const QMetaObject *getMetaObject() const = 0;
};

}

#define QTJSON_SERIALIZABLE_GADGET(Type) \
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

#endif // QTJSON_SERIALIZABLEGADGET_H
