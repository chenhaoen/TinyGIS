#include "Project.h"

#include <QDomDocument>
#include <QTextCodec>

#include <qgslayertree.h>
#include <qgsrasterlayer.h>
#include <qgsvectorlayer.h>

Project* Project::sInstance = nullptr;

Project* Project::instance()
{
	if (!sInstance)
	{
		sInstance = new Project();
	}
	return sInstance;
}

void Project::reset()
{
	m_name = m_untitledProject;
	m_file.clear();

	for (QgsLayerTreeLayer* treeLayer : m_layerTree->findLayers())
	{
		delete treeLayer->layer();
	}
	m_layerTree->removeAllChildren();
	emit layerTreeChanged();
}

bool Project::read()
{
	QFile file(m_file);
	if (!file.open(QFile::ReadOnly))
	{
		return false;
	}

	QTextStream stream(&file);
	QTextCodec* codec = QTextCodec::codecForName("UTF-8");
	stream.setCodec(codec);
	const QByteArray& content = stream.readAll().toUtf8();
	file.close();

	QDomDocument doc;
	QString errorMsg;
	int errorLine = -1;
	int errorColumn = -1;
	if (!doc.setContent(content, &errorMsg, &errorLine, &errorColumn))
	{
		return false;
	}

	const auto& LayerTreeNodeList = doc.elementsByTagName(m_elementLayerTree);
	if (LayerTreeNodeList.isEmpty())
	{
		return false;
	}
	const QDomNode& layerTreeNode = LayerTreeNodeList.item(0);

	const auto& layerTreeChildNodes = layerTreeNode.childNodes();

	for (int i = 0; i < layerTreeChildNodes.size(); ++i)
	{
		const QDomElement& layerElement = layerTreeChildNodes.item(i).toElement();
		if (layerElement.isNull())
		{
			continue;
		}

		bool ok = false;
		const QgsMapLayerType type = static_cast<QgsMapLayerType>(layerElement.attribute(m_elementType).toInt(&ok));
		if (!ok)
		{
			continue;
		}

		const QString& layerName = layerElement.attribute(m_elementName);
		const QString& layerFile = layerElement.attribute(m_elementFile);

		QSignalBlocker signalBlocker(this);
		switch (type)
		{
		case QgsMapLayerType::RasterLayer:
		{
			QgsRasterLayer* layer = new QgsRasterLayer(layerFile, layerName);

			if (!layer->isValid())
			{
				continue;
			}

			addLayer(layer);
		}
		break;
		case QgsMapLayerType::VectorLayer:
		{
			QgsVectorLayer* layer = new QgsVectorLayer(layerFile, layerName);

			if (!layer->isValid())
			{
				continue;
			}

			addLayer(layer);
		}
		break;
		default:
		break;
		}
	}

	emit layerTreeChanged();

	return true;
}

bool Project::write() const
{
	QFile file(m_file);
	if (!file.open(QFile::WriteOnly))
	{
		return false;
	}

	QDomDocument doc;

	QDomProcessingInstruction version;
	version = doc.createProcessingInstruction("xml", R"(version = "1.0" encoding = "UTF-8")");
	doc.appendChild(version);

	QDomElement domRootElement = doc.createElement(m_elementLayerTree);
	doc.appendChild(domRootElement);

	for (const auto& layer : m_layerTree->findLayers())
	{
		writeLayer(domRootElement, layer);
	}

	for (const auto& group : m_layerTree->findGroups())
	{
		QDomElement groupElement = doc.createElement(m_elementGroup);
		domRootElement.appendChild(groupElement);

		for (const auto& node : group->findLayers())
		{
			writeLayer(domRootElement, node);
		}
	}

	file.write(doc.toString().toUtf8());
	file.close();
	return true;
}

void  Project::addLayer(QgsMapLayer* layer)
{
	if (!layer||!layer->isValid())
	{
		return;
	}

	m_layerTree->addLayer(layer);
	emit layerTreeChanged();
}

Project::Project(QObject* parent)
	:QObject(parent)
	, m_layerTree(new QgsLayerTree())
	, m_elementLayerTree("LayerTree")
	, m_elementName("Name")
	, m_elementFile("File")
	, m_elementGroup("Group")
	, m_elementLayer("Layer")
	, m_elementType("Type")
	, m_untitledProject(tr("Untitled Project"))
	, m_name(m_untitledProject)
{
}

Project::~Project()
{
	delete m_layerTree;
}

void Project::writeLayer(QDomElement& parent, QgsLayerTreeLayer* layer) const
{
	if (!layer)
	{
		return;
	}

	QDomDocument doc;
	QDomElement layerElement = doc.createElement(m_elementLayer);
	parent.appendChild(layerElement);

	layerElement.setAttribute(m_elementName, layer->name());
	layerElement.setAttribute(m_elementFile, layer->layer()->dataProvider()->dataSourceUri());
	QString Type = QString::number(static_cast<int>(layer->layer()->type()));
	layerElement.setAttribute(m_elementType, Type);
}
