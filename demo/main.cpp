#include <QCoreApplication>
#include <QDebug>
#include <qtjson.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const auto test = QStringLiteral("test");
    qDebug() << QtJson::stringify(test);
    qDebug() << QtJson::binarify(test);

    return EXIT_SUCCESS;
}
