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

    const auto str = QtJson::stringify(g1);
    const auto bin = QtJson::binarify(g1);

	qDebug() << "Input gadget:" << g1;
    qDebug().noquote().nospace()
        << "Serialized JSON:\n"
        << str
        << "\nSeriailize CBOR (diagnostic notation):\n"
        << QtJson::toCbor(g1).toDiagnosticNotation(QCborValue::LineWrapped | QCborValue::ExtendedFormat)
        << "\nSeriailize CBOR (QDebug-quoted):";
    qDebug() << bin;

    qDebug() << "\n-------------\n";

    const auto g2 = QtJson::parseString<DemoGadget>(str);
    const auto g3 = QtJson::parseBinary<DemoGadget>(bin);
    qDebug() << "JSON-parsed gadget:" << g2;
    qDebug() << "CBOR-parsed gadget:" << g3;

    qDebug() << "\n-------------\n";

    qDebug().noquote().nospace()
        << "JSON-List:\n"
        << QtJson::stringify(QList<DemoGadget>{g1, g2, g3});

	return EXIT_SUCCESS;
}
