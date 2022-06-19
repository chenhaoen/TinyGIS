#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TinyGIS.h"

class QgsMapCanvas;

class TinyGIS : public QMainWindow
{
    Q_OBJECT

public:
    TinyGIS(QWidget *parent = Q_NULLPTR);

private slots:
    void slotAddRasterLayer();
    void slotAddVectorLayer();


private:
    Ui::TinyGISClass ui;

    QgsMapCanvas* m_mapCanvas;
};
