#include <QtWidgets/QApplication>

#include "TinyGIS.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
 
    TinyGIS::instance()->show();

    return a.exec();
}
