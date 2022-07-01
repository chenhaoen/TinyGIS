#pragma once

#include <QtWidgets/QMainWindow>

namespace Ui
{
	class TinyGISClass;
}

class QDockWidget;
class QgsMapCanvas;
class QgsLayerTreeView;
class QgsLayerTreeModel;
class QgsMapToolPan;
class QgsMapToolZoom;
class QTextEdit;

class TinyGIS : public QMainWindow
{
	Q_OBJECT

public:
	static  TinyGIS* instance();

	void addLog(const QString& text);

protected:
	void changeEvent(QEvent*) override;

private slots:
	void on_action_New_triggered();
	void on_action_Open_triggered();
	void on_action_Save_triggered();
	void on_action_Close_triggered();
	void on_action_Exit_TinyGIS_triggered();

	void on_action_Pan_Map_triggered();
	void on_actionPan_Map_To_Selection_triggered();
	void on_actionZoom_In_triggered();
	void on_actionZoom_Out_triggered();

	void on_actionAdd_Raster_Layer_triggered();
	void on_actionAdd_Vector_Layer_triggered();

	void on_action_Options_triggered();

	void on_actionAbout_TinyGIS_triggered();
private:
	TinyGIS(QWidget* parent = Q_NULLPTR);
	~TinyGIS();

	void refreshMapCanvas();

	void setWindowTitle();

	bool saveProject();
	void closeProject();

	bool windowModified();

	void connectAll();

	void retranslateUi();
private:
	static TinyGIS* sInstance;

	Ui::TinyGISClass* ui;

	QgsMapCanvas* m_mapCanvas;
	QgsLayerTreeView* m_layerTreeView;
	QgsLayerTreeModel* m_layerTreeModel;

 	std::unique_ptr< QgsMapToolPan> m_mapToolPan;
	std::unique_ptr < QgsMapToolZoom> m_mapToolZoomIn;
	std::unique_ptr < QgsMapToolZoom> m_mapToolZoomOut;

	QDockWidget* m_dockWidgetLayerTreeView;

	QTextEdit* m_textEditLog;
	QDockWidget* m_dockWidgetLog;
};
