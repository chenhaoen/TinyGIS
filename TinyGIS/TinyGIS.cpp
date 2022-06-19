#include "TinyGIS.h"

#include <qgsmapcanvas.h>
#include <qgslayertree.h>
#include <qgslayertreeview.h>
#include <qgslayertreemodel.h>
#include "qgsgui.h"
#include "qgssourceselectprovider.h"
#include "qgssourceselectproviderregistry.h"

TinyGIS::TinyGIS(QWidget* parent)
	: QMainWindow(parent)
	, m_layerTree(new QgsLayerTree())
	, m_mapCanvas(new QgsMapCanvas(this))
	, m_layerTreeModel(new QgsLayerTreeModel(m_layerTree, this))
	, m_layerTreeView(new QgsLayerTreeView(this))
{
	ui.setupUi(this);

	m_layerTreeView->setModel(m_layerTreeModel);

	ui.dockWidget->setWidget(m_layerTreeView);

	setCentralWidget(m_mapCanvas);
}

TinyGIS::~TinyGIS()
{
	delete m_layerTree;
}

void TinyGIS::on_actionAdd_Raster_Layer_triggered()
{
	dataSourceManager("gdal");
}

void TinyGIS::on_actionAdd_Vector_Layer_triggered()
{
	dataSourceManager("ogr");
}

void TinyGIS::on_actionAbout_Qt_triggered()
{
	QApplication::aboutQt();
}

void TinyGIS::on_actionAbout_QGIS_triggered()
{

}

void TinyGIS::dataSourceManager(const QString& providerKey)
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
			dlg->exec();
			delete dlg;
			return;
		}
		else
		{
			delete dlg;
		}
	}
}
