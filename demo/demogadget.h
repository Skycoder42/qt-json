#pragma once

#include <qtjson.h>

class DemoGadget : public QtJson::SerializableGadget
{
	Q_GADGET
	QTJSON_SERIALIZABLE_GADGET(DemoGadget)

	Q_PROPERTY(int id MEMBER id USER true)
	QTJSON_SERIALIZABLE_PROP_MEMBER(id, id, int);

	QTJSON_PROP(name, QString);
	QTJSON_PROP(when, std::optional<QDateTime>) = std::nullopt;
	QTJSON_PROP(elements, QList<QByteArray>);
	Q_PROPERTY(FunnyEnum funny MEMBER funny)

	Q_PROPERTY(bool valid READ isValid STORED false);

	Q_PROPERTY(double special READ special WRITE setSpecial)
	QTJSON_SERIALIZABLE_PROP(special, special, setSpecial, double)

public:
	enum FunnyEnum {
		This,
		Is,
		Not,
		Funny
	};
	Q_ENUM(FunnyEnum)

	int id = 0;
	FunnyEnum funny = Funny;

	bool isValid() const;
	double special() const;

public slots:
	void setSpecial(double special);

private:
	double _special = -1;
};

template <typename T>
inline QDebug operator<<(QDebug debug, const std::optional<T> &opt) {
	if (opt)
		debug << *opt;
	else
		debug << "null";
	return debug;
}

QDebug operator<<(QDebug debug, const DemoGadget &gadget);

Q_DECLARE_METATYPE(std::optional<QDateTime>)
Q_DECLARE_METATYPE(DemoGadget)
