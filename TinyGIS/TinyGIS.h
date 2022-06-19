#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TinyGIS.h"

class QgsMapCanvas;
class QgsLayerTreeView;
class QgsLayerTreeModel;
class QgsLayerTree;

class TinyGIS : public QMainWindow
{
    Q_OBJECT

public:
    TinyGIS(QWidget *parent = Q_NULLPTR);
    ~TinyGIS();

private slots:
    void on_actionAdd_Raster_Layer_triggered();
    void on_actionAdd_Vector_Layer_triggered();

    void on_actionAbout_Qt_triggered();
    void on_actionAbout_QGIS_triggered();

private:
    void dataSourceManager(const QString& providerKey);

private:
    Ui::TinyGISClass ui;

    QgsMapCanvas* m_mapCanvas;
    QgsLayerTree* m_layerTree;
    QgsLayerTreeView* m_layerTreeView;
    QgsLayerTreeModel* m_layerTreeModel;
};
