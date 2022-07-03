#include <QtWidgets/QApplication>

#include "TinyGIS.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    TinyGIS::instance()->show();
    
    const QStringList& arguments = a.arguments();
    if (arguments.size()>1)
    {
        TinyGIS::instance()->openProject(arguments[1]);
    }

    return a.exec();
}
