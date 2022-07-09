#include "TinyGIS.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDockWidget>
#include <QTextEdit>

#include <qgsmapcanvas.h>
#include <qgslayertree.h>
#include <qgslayertreeview.h>
#include <qgslayertreemodel.h>
#include <qgsrasterlayer.h>
#include <qgsvectorlayer.h>
#include <qgsmaptoolpan.h>
#include <qgsmaptoolzoom.h>

#include "qgis/qgsmeasuretool.h"

#include "AboutTinyGISDlg.h"
#include "ui_TinyGIS.h"
#include "OptionsDlg.h"
#include "Project.h"

TinyGIS* TinyGIS::sInstance = nullptr;

TinyGIS::TinyGIS(QWidget* parent)
	: QMainWindow(parent, Qt::WindowType::Window)
	, ui(new Ui::TinyGISClass)
	, m_mapCanvas(new QgsMapCanvas(this))
	, m_layerTreeModel(new QgsLayerTreeModel(Project::instance()->layerTree(), this))
	, m_layerTreeView(new QgsLayerTreeView(this))
	, m_dockWidgetLayerTreeView(new QDockWidget(this))
	, m_dockWidgetLog(new QDockWidget(this))
	, m_textEditLog(new QTextEdit(this))
{
	ui->setupUi(this);

	m_textEditLog->setReadOnly(true);
	m_dockWidgetLog->setWidget(m_textEditLog);
	addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, m_dockWidgetLog);

	m_layerTreeModel->setFlag(QgsLayerTreeModel::AllowNodeRename);
	m_layerTreeModel->setFlag(QgsLayerTreeModel::AllowNodeChangeVisibility);
	m_layerTreeModel->setFlag(QgsLayerTreeModel::ShowLegendAsTree);
	m_layerTreeModel->setFlag(QgsLayerTreeModel::UseEmbeddedWidgets);
	m_layerTreeModel->setFlag(QgsLayerTreeModel::UseTextFormatting);
	m_layerTreeModel->setAutoCollapseLegendNodes(10);
	m_layerTreeView->setModel(m_layerTreeModel);
	m_dockWidgetLayerTreeView->setWidget(m_layerTreeView);
	addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_dockWidgetLayerTreeView);

	retranslateUi();
	on_action_New_triggered();
	setCentralWidget(m_mapCanvas);
	showMaximized();
	setAcceptDrops(true);

	connectAll();
}

TinyGIS::~TinyGIS()
{
	delete ui;
}

TinyGIS* TinyGIS::instance()
{
	if (!sInstance)
	{
		sInstance = new TinyGIS();
	}
	return sInstance;
}

void TinyGIS::addLog(const QString& text)
{
	QString newString = m_textEditLog->toPlainText();

	newString += '[' + QDateTime::currentDateTime().toString() + ']' + text + '\n';

	m_textEditLog->setText(newString);
}

void TinyGIS::changeEvent(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		retranslateUi();
		break;
	default:
		QMainWindow::changeEvent(event);
	}
}

void TinyGIS::dragEnterEvent(QDragEnterEvent* event)
{
	const QList<QUrl>& urls = event->mimeData()->urls();

	if (urls.isEmpty())
	{
		QMainWindow::dragEnterEvent(event);
		return;
	}

	bool allUrlIsValid = true;
	for (const QUrl& url : urls)
	{
		QFileInfo fileInfo(url.toLocalFile());

		const QString& suffix = fileInfo.suffix();
		if (suffix != "gpkg" && suffix != "shp" && suffix != "tif" && suffix != "tiff")
		{
			allUrlIsValid = false;
			break;
		}
	}

	if (allUrlIsValid)
	{
		event->acceptProposedAction();
	}
}

void TinyGIS::dropEvent(QDropEvent* event)
{
	const QList<QUrl>& urls = event->mimeData()->urls();

	for (const QUrl& url : urls)
	{
		QFileInfo fileInfo(url.toLocalFile());

		const QString& suffix = fileInfo.suffix();
		//raster file
		if (suffix == "gpkg" || suffix == "shp")
		{
			QgsVectorLayer* layer = new QgsVectorLayer(fileInfo.absoluteFilePath(), fileInfo.baseName());

			Project::instance()->addLayer(layer);
		}

		if (suffix == "tif" || suffix == "tiff")
		{
			QgsRasterLayer* layer = new QgsRasterLayer(fileInfo.absoluteFilePath(), fileInfo.baseName());

			Project::instance()->addLayer(layer);
		}
	}
}

void TinyGIS::on_action_New_triggered()
{
	if (!windowModified())
	{
		return;
	}

	closeProject();
	setWindowTitle();
	setWindowModified(false);
}

void TinyGIS::on_action_Open_triggered()
{
	if (!windowModified())
	{
		return;
	}

	closeProject();

	const QString& fileName = QFileDialog::getOpenFileName(this, tr("Open Project File"), {}, QString("TinyGIS Project File(*.xml)"));
	if (fileName.isEmpty())
	{
		return;
	}

	openProject(fileName);
}

void TinyGIS::on_action_Save_triggered()
{
	if (!saveProject())
	{
		return;
	}

	addLog(tr("Save project successfully."));
	setWindowModified(false);
	setWindowTitle();
}

void TinyGIS::on_action_Close_triggered()
{
	on_action_New_triggered();
	addLog(tr("Close project successfully."));
}

void TinyGIS::on_action_Exit_TinyGIS_triggered()
{
	qApp->exit();
}

void TinyGIS::on_action_Pan_Map_triggered()
{
	if (!m_mapToolPan)
	{
		m_mapToolPan = std::make_unique<QgsMapToolPan>(m_mapCanvas);
		m_mapToolPan->setAction(ui->action_Pan_Map);
	}

	m_mapCanvas->setMapTool(m_mapToolPan.get());
}

void TinyGIS::on_actionPan_Map_To_Selection_triggered()
{
	QgsMapLayer* currentLayer = m_mapCanvas->currentLayer();

	if (!currentLayer || !currentLayer->isValid())
	{
		return;
	}

	m_mapCanvas->setExtent(currentLayer->extent());
	m_mapCanvas->refresh();
}

void TinyGIS::on_actionZoom_In_triggered()
{
	if (!m_mapToolZoomIn)
	{
		m_mapToolZoomIn = std::make_unique < QgsMapToolZoom>(m_mapCanvas, false);
		m_mapToolZoomIn->setAction(ui->actionZoom_In);
	}

	m_mapCanvas->setMapTool(m_mapToolZoomIn.get());
}

void TinyGIS::on_actionZoom_Out_triggered()
{
	if (!m_mapToolZoomOut)
	{
		m_mapToolZoomOut = std::make_unique < QgsMapToolZoom>(m_mapCanvas, true);
		m_mapToolZoomOut->setAction(ui->actionZoom_Out);
	}

	m_mapCanvas->setMapTool(m_mapToolZoomOut.get());
}

void TinyGIS::on_actionZoom_Last_triggered()
{
	m_mapCanvas->zoomToPreviousExtent();
}

void TinyGIS::on_actionZoom_Next_triggered()
{
	m_mapCanvas->zoomToNextExtent();
}

void TinyGIS::on_actionRefresh_triggered()
{
	m_mapCanvas->refresh();
}

void TinyGIS::on_actionMeasure_Line_triggered()
{
	if (!m_mapToolMeasureLine)
	{
		m_mapToolMeasureLine = std::make_unique < QgsMeasureTool>(m_mapCanvas, false);
		m_mapToolMeasureLine->setAction(ui->actionZoom_Out);
	}

	m_mapCanvas->setMapTool(m_mapToolMeasureLine.get());
}

void TinyGIS::on_actionMeasure_Area_triggered()
{
	if (!m_mapToolMeasureArea)
	{
		m_mapToolMeasureArea = std::make_unique < QgsMeasureTool>(m_mapCanvas, true);
		m_mapToolMeasureArea->setAction(ui->actionZoom_Out);
	}

	m_mapCanvas->setMapTool(m_mapToolMeasureArea.get());
}

void TinyGIS::on_actionAdd_Raster_Layer_triggered()
{
	const QStringList& fileNames = QFileDialog::getOpenFileNames(this, tr("Add raster layer"), QString(), QString("GTiff(*.tif;*.tiff)"));

	if (fileNames.isEmpty())
	{
		return;
	}

	QSignalBlocker signalBlocker(Project::instance()->layerTree());
	for (const QString& fileName : fileNames)
	{
		QFileInfo fileInfo(fileName);
		QgsRasterLayer* layer = new QgsRasterLayer(fileName, fileInfo.baseName());

		Project::instance()->addLayer(layer);
	}

	refreshMapCanvas();
}

void TinyGIS::on_actionAdd_Vector_Layer_triggered()
{
	const QStringList& fileNames = QFileDialog::getOpenFileNames(this, tr("Add vector layer"), QString(), QString("ESRI Shapefile(*.shp);;GeoPackage(*.gpkg)"));

	if (fileNames.isEmpty())
	{
		return;
	}

	QSignalBlocker signalBlocker(Project::instance()->layerTree());
	for (const QString& fileName : fileNames)
	{
		QFileInfo fileInfo(fileName);
		QgsVectorLayer* layer = new QgsVectorLayer(fileName, fileInfo.baseName());

		Project::instance()->addLayer(layer);
	}

	refreshMapCanvas();
}

void TinyGIS::on_action_Options_triggered()
{
	OptionsDlg dlg(this);
	dlg.exec();
}

void TinyGIS::on_actionAbout_TinyGIS_triggered()
{
	AboutTinyGISDlg dlg(this);
	dlg.exec();
}

void TinyGIS::on_layerTree_ItemPressed(const QModelIndex& index)
{
	QgsMapLayer* layer = m_layerTreeView->currentLayer();

	if (!layer && !layer->isValid())
	{
		return;
	}

	if (qApp->mouseButtons() == Qt::MouseButton::RightButton)
	{
		QMenu menu;

		QAction* actionZoomToLayer = new QAction(tr("Zoom to Layer"), &menu);
		connect(actionZoomToLayer, &QAction::triggered, this, &TinyGIS::on_actionPan_Map_To_Selection_triggered);

		QAction* actionRemoveLayer = new QAction(QIcon(":/TinyGIS/images/remove"), tr("Remove layer"), &menu);
		connect(actionRemoveLayer, &QAction::triggered, this, [this, layer] {Project::instance()->removeLayer(layer); });

		menu.addAction(actionZoomToLayer);
		menu.addAction(actionRemoveLayer);

		menu.exec(QCursor::pos());
	}
}

void TinyGIS::refreshMapCanvas()
{
	m_mapCanvas->setLayers(Project::instance()->layerTree()->checkedLayers());
	m_mapCanvas->refresh();
}

void TinyGIS::setWindowTitle()
{
	 QString projectName = Project::instance()->name();

	if (projectName.isEmpty())
	{
		projectName = tr("Untitled Project");
	}

	QMainWindow::setWindowTitle(tr("[*]%1 - TinyGIS").arg(projectName));
}

bool TinyGIS::saveProject()
{
	if (Project::instance()->file().isEmpty())
	{
		const QString& fileName = QFileDialog::getSaveFileName(this, tr("Save Project File"), QString(), QString("TinyGIS Project File(*.xml)"));

		if (fileName.isEmpty())
		{
			return false;
		}

		QFileInfo fileInfo(fileName);
		Project::instance()->setFile(fileName);
		Project::instance()->setName(fileInfo.baseName());
	}

	if (!Project::instance()->write())
	{
		return false;
	}

	return true;
}

void TinyGIS::closeProject()
{
	Project::instance()->reset();
}

bool TinyGIS::windowModified()
{
	if (!isWindowModified())
	{
		return true;
	}

	switch (QMessageBox::information(this, QString(), tr("Save current project?"), QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel))
	{
	case  QMessageBox::Ok:
		saveProject();
		break;
	case  QMessageBox::No:
		break;
	case  QMessageBox::Cancel:
		return false;
	default:
		break;
	}

	return true;
}

void TinyGIS::connectAll()
{
	connect(m_layerTreeView, &QgsLayerTreeView::currentLayerChanged, m_mapCanvas, &QgsMapCanvas::setCurrentLayer);
	connect(m_layerTreeView, &QAbstractItemView::pressed, this, &TinyGIS::on_layerTree_ItemPressed);

	auto lambdaSetModified = [this] { setWindowModified(true); };
	connect(m_layerTreeView->layerTreeModel()->rootGroup(), &QgsLayerTreeNode::addedChildren,
		this, lambdaSetModified);
	connect(m_layerTreeView->layerTreeModel()->rootGroup(), &QgsLayerTreeNode::addedChildren,
		this, &TinyGIS::refreshMapCanvas);
	connect(m_layerTreeView->layerTreeModel()->rootGroup(), &QgsLayerTreeNode::removedChildren,
		this, lambdaSetModified);
	connect(m_layerTreeView->layerTreeModel()->rootGroup(), &QgsLayerTreeNode::addedChildren,
		this, &TinyGIS::refreshMapCanvas);
}

void TinyGIS::retranslateUi()
{
	m_dockWidgetLayerTreeView->setWindowTitle(tr("Layer View"));
	m_dockWidgetLog->setWindowTitle(tr("Log"));
	setWindowTitle();
}

void TinyGIS::openProject(const QString& fileName)
{
	QFileInfo fileInfo(fileName);
	if (!fileInfo.exists() || fileInfo.suffix() != "xml")
	{
		addLog(tr("The project file is not a valid file."));
		return;
	}

	closeProject();
	Project::instance()->setFile(fileName);
	Project::instance()->setName(fileInfo.baseName());

	if (Project::instance()->read())
	{
		addLog(tr("Open project successfully."));
	}
	else
	{
		addLog(tr("Failed to open project."));
	}

	setWindowTitle();
	setWindowModified(false);
}
