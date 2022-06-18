#include "TinyGIS.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TinyGIS w;
    w.show();
    return a.exec();
}
