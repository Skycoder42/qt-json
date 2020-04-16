#ifndef QTJSON_SERIALIZABLEGADGET_H
#define QTJSON_SERIALIZABLEGADGET_H

#include "qtjson_global.h"
#include "iserializable.h"
#include "qtjson_metamacros.h"

#include <QtCore/QObject>

namespace QtJson {

class SerializableGadget : public ISerializable
{
public:
    QJsonValue toJson() const override;
    void assignJson(const QJsonValue &value) override;
    QCborValue toCbor() const override;
    void assignCbor(const QCborValue &value) override;

protected:
    virtual const QMetaObject *getMetaObject() const = 0;
};

}

#endif // QTJSON_SERIALIZABLEGADGET_H
