#include "TinyGIS.h"

#include <QFileDialog>
#include <QMessageBox>

#include <qgsmapcanvas.h>
#include <qgslayertree.h>
#include <qgslayertreeview.h>
#include <qgslayertreemodel.h>
#include <qgsgui.h>
#include <qgssourceselectprovider.h>
#include <qgssourceselectproviderregistry.h>
#include <qgsrasterlayer.h>
#include <qgsvectorlayer.h>
#include <qgsmaptoolpan.h>
#include <qgsmaptoolzoom.h>

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
{
	ui->setupUi(this);

	m_layerTreeView->setModel(m_layerTreeModel);
	ui->dockWidget->setWidget(m_layerTreeView);

	on_action_New_triggered();
	setCentralWidget(m_mapCanvas);
	showMaximized();

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

void TinyGIS::changeEvent(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		setWindowTitle();
		break;
	default:
		QMainWindow::changeEvent(event);
	}
}

void TinyGIS::on_action_New_triggered()
{
	if (!windowModified())
	{
		return;
	}

	closeProject();
	refreshMapCanvas();
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

	QFileInfo fileInfo(fileName);
	Project::instance()->setFile(fileName);
	Project::instance()->setName(fileInfo.baseName());
	Project::instance()->read();

	refreshMapCanvas();
	setWindowTitle();
	setWindowModified(false);
}

void TinyGIS::on_action_Save_triggered()
{
	if (!saveProject())
	{
		return;
	}

	setWindowModified(false);
}

void TinyGIS::on_action_Close_triggered()
{
	on_action_New_triggered();
}

void TinyGIS::on_action_Exit_TinyGIS_triggered()
{
	qApp->exit();
}

void TinyGIS::on_action_Pan_Map_triggered()
{
	if (!m_mapToolPan)
	{
		m_mapToolPan =std::make_unique<QgsMapToolPan>(m_mapCanvas);
		m_mapToolPan->setAction(ui->action_Pan_Map);
   }

	m_mapCanvas->setMapTool(m_mapToolPan.get());
}

void TinyGIS::on_actionPan_Map_To_Selection_triggered()
{
	QgsMapLayer* currentLayer = m_mapCanvas->currentLayer();

	if (!currentLayer)
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
		m_mapToolZoomIn =  std::make_unique < QgsMapToolZoom>(m_mapCanvas, false);
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

void TinyGIS::on_actionAdd_Raster_Layer_triggered()
{
	QgsAbstractDataSourceWidget* dlg = getDataSourceWidget("gdal");

	if (!dlg)
	{
		return;
	}

	connect(dlg, &QgsAbstractDataSourceWidget::addRasterLayers, this, &TinyGIS::slotAddRasterLayers);

	dlg->exec();
	delete dlg;
}

void TinyGIS::on_actionAdd_Vector_Layer_triggered()
{
	QgsAbstractDataSourceWidget* dlg = getDataSourceWidget("ogr");

	if (!dlg)
	{
		return;
	}

	connect(dlg, &QgsAbstractDataSourceWidget::addVectorLayers, this, &TinyGIS::slotAddVectorLayers);

	dlg->exec();
	delete dlg;
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

void TinyGIS::slotAddRasterLayers(const QStringList& layersList)
{
	if (layersList.empty())
	{
		return;
	}

	for (const QString& layerFile : layersList)
	{
		QFileInfo fileInfo(layerFile);
		QgsRasterLayer* layer = new QgsRasterLayer(layerFile, fileInfo.baseName());

		Project::instance()->layerTree()->addLayer(layer);
	}
	setWindowModified(true);
	refreshMapCanvas();
}

void TinyGIS::slotAddVectorLayers(const QStringList& layerList, const QString& encoding, const QString& dataSourceType)
{
	if (dataSourceType != "file")
	{
		return;
	}

	for (const QString& layerPath : layerList)
	{
		QFileInfo fileInfo(layerPath);
		QgsVectorLayer* layer = new QgsVectorLayer(layerPath, fileInfo.baseName());

		Project::instance()->layerTree()->addLayer(layer);
	}

	setWindowModified(true);
	refreshMapCanvas();
}

QgsAbstractDataSourceWidget* TinyGIS::getDataSourceWidget(const QString& providerKey)
{
	const QList<QgsSourceSelectProvider*>& sourceSelectProviders = QgsGui::sourceSelectProviderRegistry()->providers();
	for (QgsSourceSelectProvider* provider : sourceSelectProviders)
	{
		QgsAbstractDataSourceWidget* dlg = provider->createDataSourceWidget(this);
		if (!dlg)
		{
			continue;
		}

		if (providerKey == provider->providerKey())
		{
			return dlg;
		}
		else
		{
			delete dlg;
		}
	}

	return nullptr;
}

void TinyGIS::refreshMapCanvas()
{
	m_mapCanvas->setLayers(Project::instance()->layerTree()->checkedLayers());
	m_mapCanvas->refresh();
}

void TinyGIS::setWindowTitle()
{
	QMainWindow::setWindowTitle(tr("[*]%1 - TinyGIS").arg(Project::instance()->name()));
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

		Project::instance()->setFile(fileName);
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
}
