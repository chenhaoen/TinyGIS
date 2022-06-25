#pragma once

#include <QtWidgets/QMainWindow>
#include <QMap>

namespace Ui
{
    class TinyGISClass;
}

class QgsMapCanvas;
class QgsLayerTreeView;
class QgsLayerTreeModel;
class QgsLayerTree;
class QgsAbstractDataSourceWidget;
class QgsMapLayer;

class TinyGIS : public QMainWindow
{
    Q_OBJECT

public:
    TinyGIS(QWidget *parent = Q_NULLPTR);
    ~TinyGIS();

protected:
    void changeEvent(QEvent*) override;

private slots:
    void on_actionAdd_Raster_Layer_triggered();
    void on_actionAdd_Vector_Layer_triggered();

    void on_action_Options_triggered();

    void on_actionAbout_TinyGIS_triggered();

    void slotAddRasterLayer(const QStringList& layersList);
private:
    QgsAbstractDataSourceWidget* getDataSourceWidget(const QString& providerKey);

private:
    Ui::TinyGISClass* ui;

    QgsMapCanvas* m_mapCanvas;
    QgsLayerTree* m_layerTree;
    QgsLayerTreeView* m_layerTreeView;
    QgsLayerTreeModel* m_layerTreeModel;

    QMap<QString, QgsMapLayer*> m_layers;
};
