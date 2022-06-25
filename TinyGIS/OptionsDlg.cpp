#include "OptionsDlg.h"

#include <QDir>
#include <QTranslator>

#include "ui_OptionsDlg.h"

OptionsDlg::OptionsDlg(QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::OptionsDlgClass())
{
	ui->setupUi(this);

	initComboBoxLanguage();
}

OptionsDlg::~OptionsDlg()
{
	delete ui;
}

void OptionsDlg::changeEvent(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		initComboBoxLanguage();
		break;
	default:
		QDialog::changeEvent(event);
	}
}

void OptionsDlg::initComboBoxLanguage()
{
	const int  lastIndex = ui->comboBoxLanguage->currentIndex();
	ui->comboBoxLanguage->clear();
	QVector<QPair<QString, QString>> languagePairs;
	languagePairs.push_back(qMakePair(tr("English"), QString("en")));
	languagePairs.push_back(qMakePair(tr("Chinese"), QString("zh_Hans")));

	QSignalBlocker signalBlocker(ui->comboBoxLanguage);
	for (const auto& pair : languagePairs)
	{
		ui->comboBoxLanguage->addItem(pair.first, pair.second);
	}

	if (-1 != lastIndex)
	{
		ui->comboBoxLanguage->setCurrentIndex(lastIndex);
	}
}

void OptionsDlg::on_comboBoxLanguage_currentIndexChanged(int index)
{
	const QString& language = ui->comboBoxLanguage->itemData(index).toString();

	if (language.isEmpty())
	{
		return;
	}

	QDir dir(qApp->applicationDirPath());
	QString namefilter = "*" + language + ".qm";
	const QFileInfoList& qmFileInfos = dir.entryInfoList(QStringList(namefilter), QDir::AllEntries);

	for (const auto translator : m_translators)
	{
		qApp->removeTranslator(translator);
	}
	m_translators.clear();

	for (const QFileInfo& qmFileInfo : qmFileInfos)
	{
		QTranslator* translator = new QTranslator(qApp);
		translator->load(qmFileInfo.absoluteFilePath());

		m_translators.append(translator);
	}

	for (const auto translator : m_translators)
	{
		qApp->installTranslator(translator);
	}
}
