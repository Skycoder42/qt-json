#include "testgadget.h"

int TestGadget::getProp3() const
{
    return prop3;
}

void TestGadget::setProp3(int value)
{
    prop3 = value;
}

TestGadget::Flags TestGadget::getProp7() const
{
	return prop7;
}

void TestGadget::setProp7(Flags value)
{
	prop7 = value;
}

bool TestGadget::operator==(const TestGadget &other) const
{
    return prop1 == other.prop1 &&
            prop2 == other.prop2 &&
		   prop3 == other.prop3 &&
		   prop4 == other.prop4 &&
		   prop5 == other.prop5 &&
		   prop6 == other.prop6 &&
		   prop7 == other.prop7 &&
		   prop8 == other.prop8;
}

QDebug operator<<(QDebug debug, const TestGadget &gadget)
{
	QDebugStateSaver state{debug};
	debug.nospace() << "TestGadget{"
					<< "p1=" << gadget.prop1 << ", "
					<< "p2=" << gadget.prop2 << ", "
					<< "p3=" << gadget.prop3 << ", "
					<< "p4=" << gadget.prop4 << ", "
					<< "p5=" << gadget.prop5 << ", "
					<< "p6=" << gadget.prop6 << ", "
					<< "p7=" << gadget.prop7 << ", "
					<< "p8=" << gadget.prop8
					<< "}";
	return debug;
}
