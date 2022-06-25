#include "TinyGIS.h"

#include <qgsmapcanvas.h>
#include <qgslayertree.h>
#include <qgslayertreeview.h>
#include <qgslayertreemodel.h>
#include <qgsgui.h>
#include <qgssourceselectprovider.h>
#include <qgssourceselectproviderregistry.h>
#include <qgsrasterlayer.h>
#include <qgsvectorlayer.h>

#include "AboutTinyGISDlg.h"
#include "ui_TinyGIS.h"
#include "OptionsDlg.h"

TinyGIS::TinyGIS(QWidget* parent)
	: QMainWindow(parent,Qt::WindowType::Window)
	, ui(new Ui::TinyGISClass)
	, m_layerTree(new QgsLayerTree())
	, m_mapCanvas(new QgsMapCanvas(this))
	, m_layerTreeModel(new QgsLayerTreeModel(m_layerTree, this))
	, m_layerTreeView(new QgsLayerTreeView(this))
{
	ui->setupUi(this);

	m_layerTreeView->setModel(m_layerTreeModel);
	ui->dockWidget->setWidget(m_layerTreeView);

	setCentralWidget(m_mapCanvas);
	showMaximized();
}

TinyGIS::~TinyGIS()
{
	delete m_layerTree;

	qDeleteAll(m_layers);
}

void TinyGIS::changeEvent(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		QMainWindow::changeEvent(event);
	}
}

void TinyGIS::on_actionAdd_Raster_Layer_triggered()
{
	QgsAbstractDataSourceWidget* dlg = getDataSourceWidget("gdal");

	connect(dlg, &QgsAbstractDataSourceWidget::addRasterLayers, this, &TinyGIS::slotAddRasterLayer);

	dlg->exec();
	delete dlg;
}

void TinyGIS::on_actionAdd_Vector_Layer_triggered()
{
	QgsAbstractDataSourceWidget* dlg = getDataSourceWidget("ogr");
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

void TinyGIS::slotAddRasterLayer(const QStringList& layersList)
{
	if (layersList.empty())
	{
		return;
   }

	for (const QString& layerFile : layersList)
	{
		QFileInfo fileInfo(layerFile);
		QgsRasterLayer* layer = new QgsRasterLayer(layerFile, fileInfo.baseName());

		m_layerTree->addLayer(layer);
	}

	m_mapCanvas->setLayers(m_layerTree->checkedLayers());
	m_mapCanvas->refresh();
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
}
