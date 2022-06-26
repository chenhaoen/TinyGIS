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
class QgsAbstractDataSourceWidget;
class QgsMapLayer;

class TinyGIS : public QMainWindow
{
    Q_OBJECT

public:
   static  TinyGIS* instance();

protected:
    void changeEvent(QEvent*) override;

private slots:
    void on_action_New_triggered();
    void on_action_Open_triggered();
    void on_action_Save_triggered();
    void on_action_Close_triggered();
    void on_action_Exit_TinyGIS_triggered();

    void on_actionAdd_Raster_Layer_triggered();
    void on_actionAdd_Vector_Layer_triggered();

    void on_action_Options_triggered();

    void on_actionAbout_TinyGIS_triggered();

    void slotAddRasterLayers(const QStringList& layersList);
    void slotAddVectorLayers(const QStringList& layerList, const QString& encoding, const QString& dataSourceType);
private:
    TinyGIS(QWidget* parent = Q_NULLPTR);
    ~TinyGIS();

    QgsAbstractDataSourceWidget* getDataSourceWidget(const QString& providerKey);

    void refreshMapCanvas();

   void setWindowTitle();

   bool SaveProject();
    void CloseProject();

    bool windowModified();
private:
    static TinyGIS* sInstance;

    Ui::TinyGISClass* ui;

    QgsMapCanvas* m_mapCanvas;
    QgsLayerTreeView* m_layerTreeView;
    QgsLayerTreeModel* m_layerTreeModel;

    QMap<QString, QgsMapLayer*> m_layers;
};
