#pragma once

#include <serializablegadget.h>

class DemoGadget : public QtJson::SerializableGadget
{
	Q_GADGET
	QTJSON_SERIALIZABLE_GADGET(DemoGadget)

	Q_PROPERTY(int id MEMBER id USER true)
	QTJSON_PROP_MEMBER(id, int) = 0;

	QTJSON_PROP(name, QString);
	QTJSON_PROP(when, std::optional<QtJson::SerializableDateTime>);
	QTJSON_PROP(elements, QtJson::SerializableList<QtJson::SerializableByteArray>);

	Q_PROPERTY(bool valid READ isValid STORED false);

	Q_PROPERTY(double special READ special WRITE setSpecial)
	QTJSON_SERIALIZABLE_PROP(special, _special, double)

public:
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

