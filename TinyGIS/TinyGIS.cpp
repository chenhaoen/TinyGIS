#include "TinyGIS.h"

#include <qgsmapcanvas.h>

TinyGIS::TinyGIS(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QgsMapCanvas* map_canvas = new QgsMapCanvas(this);

    setCentralWidget(map_canvas);

}
