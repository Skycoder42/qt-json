#include <QtCore>
#include "demogadget.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	DemoGadget g1;
	g1.id = 42;
    g1.name = QStringLiteral("g1");
    g1.when = QDateTime::currentDateTime();
    g1.elements = {"e1", "e2", "e3"};

	return EXIT_SUCCESS;
}
