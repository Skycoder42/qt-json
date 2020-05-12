#include <QtCore>
#include "demogadget.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	DemoGadget g1;
	g1.id = 42;
	g1.name = QStringLiteral("g1");
	g1.when = std::optional<QtJson::SerializableDateTime>(QDateTime::currentDateTime());
	g1.elements = {
		QtJson::SerializableByteArray{""}
	};

	return EXIT_SUCCESS;
}
