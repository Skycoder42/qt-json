#include <QtCore>
#include <qtjson.h>
#include "demogadget.h"

Q_DECLARE_OPERATORS_FOR_FLAGS(QCborValue::DiagnosticNotationOptions)

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	DemoGadget g1;
	g1.id = 42;
	g1.name = QStringLiteral("g1");
	g1.when = QDateTime::currentDateTime();
	g1.elements = {"e1", "e2", "e3"};
	g1.setSpecial(1.2);

	const auto json = QtJson::toJson(g1);
	const auto cbor = QtJson::toCbor(g1);

	qDebug() << "Input gadget:" << g1;
	qDebug().noquote() << "Serialized JSON:\n"
					   << QtJson::stringify(g1);
	qDebug().noquote() << "Seriailize CBOR:\n"
					   << cbor.toDiagnosticNotation(QCborValue::LineWrapped | QCborValue::ExtendedFormat);

	return EXIT_SUCCESS;
}
