#pragma once

#include <serializablegadget.h>
#include <serializablebytearray.h>

class TestGadget : public QtJson::SerializableGadget
{
	Q_GADGET
	QTJSON_SERIALIZABLE_GADGET(TestGadget)

public:
	enum class Enum {
		Value0,
		Value1,
		Value3 = 3
	};
	Q_ENUM(Enum)

	enum class Flag {
		Flag1 = 0x01,
		Flag2 = 0x02,
		Flag4 = 0x04,

		Flag5 = (Flag1 | Flag4)
	};
	Q_DECLARE_FLAGS(Flags, Flag)
	Q_FLAG(Flags)

	Q_PROPERTY(int prop1 MEMBER prop1 STORED true)

	Q_PROPERTY(int prop2 MEMBER prop2 STORED false)

	Q_PROPERTY(int prop3 MEMBER prop3)
	QTJSON_SERIALIZABLE_PROP(prop3, prop3, int)

	Q_PROPERTY(int prop4 MEMBER prop4)
	QTJSON_PROP_MEMBER(prop4, int) = 4;

	QTJSON_PROP(prop5, double) {};

	QTJSON_PROP(prop6, QByteArray);

	Q_PROPERTY(Flags prop7 READ getProp7 WRITE setProp7)

	QTJSON_PROP(prop8, Enum) = Enum::Value1;

public:
	int prop1 = 1;
	int prop2 = 2;
	int prop3 = 3;
	Flags prop7 = 0;

	bool operator==(const TestGadget &other) const;

	Flags getProp7() const;
	void setProp7(Flags value);
};

QDebug operator<<(QDebug debug, const TestGadget &gadget);

Q_DECLARE_OPERATORS_FOR_FLAGS(TestGadget::Flags)
