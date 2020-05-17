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
	debug.noquote().nospace() << "DemoGadget{"
							  << "id=" << gadget.id << ", "
							  << "name=" << gadget.name << ", "
							  << "when=" << gadget.when << ", "
							  << "elements=" << gadget.elements << ", "
							  << "funny=" << gadget.funny << ", "
							  << "special=" << gadget.special() << ", "
							  << "valid=" << gadget.isValid() << "}";
	return debug;
}
