#include "TinyGIS.h"

#include <QFileDialog>

#include <qgsmapcanvas.h>
#include <qgsrasterlayer.h>
#include <qgsvectorlayer.h>

TinyGIS::TinyGIS(QWidget* parent)
	: QMainWindow(parent)
	, m_mapCanvas(new QgsMapCanvas(this))
{
	ui.setupUi(this);

	setCentralWidget(m_mapCanvas);

	connect(ui.actionAdd_Raster_Layer, &QAction::triggered, this, &TinyGIS::slotAddRasterLayer);
	connect(ui.actionAdd_Vector_Layer, &QAction::triggered, this, &TinyGIS::slotAddVectorLayer);
}

void TinyGIS::slotAddRasterLayer()
{
	const QString& fileName = QFileDialog::getOpenFileName(this, QStringLiteral("Ìí¼ÓÕ¤¸ñÍ¼²ã"), {}, QString("Geo Tiff(*.tif)"));

	if (fileName.isEmpty())
	{
		return;
	}

	QgsRasterLayer* layer = new QgsRasterLayer(fileName);
	auto layers = m_mapCanvas->layers();
	layers.append(layer);
	m_mapCanvas->setLayers(layers);
	m_mapCanvas->setExtent(layer->extent());
	m_mapCanvas->refresh();
}

void TinyGIS::slotAddVectorLayer()
{
	const QString& fileName = QFileDialog::getOpenFileName(this, QStringLiteral("Ìí¼ÓÊ¸Á¿Í¼²ã"), {}, QString("Shape file(*.shp)"));

	if (fileName.isEmpty())
	{
		return;
	}

	QgsVectorLayer* layer = new QgsVectorLayer(fileName);
	auto layers = m_mapCanvas->layers();
	layers.append(layer);
	m_mapCanvas->setLayers(layers);
	m_mapCanvas->setExtent(layer->extent());
	m_mapCanvas->refresh();
}
