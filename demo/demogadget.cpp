#include "demogadget.h"

bool DemoGadget::isValid() const {
	return id != 0;
}

double DemoGadget::special() const {
	return _special;
}

void DemoGadget::setSpecial(double special) {
	_special = special;
}

QDebug operator<<(QDebug debug, const DemoGadget &gadget)
{
	QDebugStateSaver state{debug};
    debug.noquote().nospace() << "DemoGadget {\n"
                              << "    id=" << gadget.id << ",\n"
                              << "    name=" << gadget.name << ",\n"
                              << "    when=" << gadget.when << ",\n"
                              << "    elements=" << gadget.elements << ",\n"
                              << "    funny=" << gadget.funny << ",\n"
                              << "    special=" << gadget.special() << ",\n"
                              << "    valid=" << gadget.isValid() << "\n}";
	return debug;
}
