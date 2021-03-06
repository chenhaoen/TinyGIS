#pragma once

#include <QObject>

class QDomElement;

class QgsLayerTree;
class QgsLayerTreeLayer;
class QgsMapLayer;

class Project :public QObject
{
	Q_OBJECT;
public:
	static Project* instance();

	void reset();

	QgsLayerTree* layerTree()
	{
		return m_layerTree;
	}

	void setFile(const QString& file)
	{
		m_file = file;
	}
	const QString& file() const
	{
		return m_file;
	}

	void setName(const QString& name)
	{
		m_name = name;
	}
	const QString& name() const
	{
		return m_name;
	}

	bool read();
	bool  write() const;

	void addLayer(QgsMapLayer*);
	void removeLayer(QgsMapLayer*);

private:
	Project(QObject* parent = nullptr);
	~Project();

	void writeLayer(QDomElement& parent, QgsLayerTreeLayer* layer) const;

private:
	static Project* sInstance;

	QgsLayerTree* m_layerTree;

	QString m_elementLayerTree;
	QString m_elementName;
	QString m_elementFile;
	QString m_elementLayer;
	QString m_elementGroup;
	QString m_elementType;

	QString m_name;
	QString m_file;
};

