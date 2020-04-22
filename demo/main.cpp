#include <QtCore>
#include <qtjson.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const QList<int> test = {1, 2, 3};
    qDebug() << QtJson::stringify(test);
    qDebug() << QtJson::binarify(test);

    return EXIT_SUCCESS;
}
