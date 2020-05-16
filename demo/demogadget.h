#pragma once

#include <serializablegadget.h>

class DemoGadget : public QtJson::SerializableGadget
{
	Q_GADGET
    QTJSON_SERIALIZABLE_GADGET(DemoGadget)

	Q_PROPERTY(int id MEMBER id USER true)
    QTJSON_SERIALIZABLE_PROP_MEMBER(id, id, int);

    QTJSON_PROP(name, QString);
    QTJSON_PROP(when, std::optional<QDateTime>) = std::nullopt;
    QTJSON_PROP(elements, QList<QByteArray>);

	Q_PROPERTY(bool valid READ isValid STORED false);

	Q_PROPERTY(double special READ special WRITE setSpecial)
    QTJSON_SERIALIZABLE_PROP(special, special, setSpecial, double)

public:
    int id = 0;

	inline bool isValid() const {
		return id != 0;
	}

	inline double special() const {
		return _special;
	}

public slots:
	inline void setSpecial(double special) {
		_special = special;
	}

private:
	double _special = -1;
};

Q_DECLARE_METATYPE(std::optional<QDateTime>)
Q_DECLARE_METATYPE(DemoGadget)
