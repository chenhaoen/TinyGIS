#pragma once

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class OptionsDlgClass; };
QT_END_NAMESPACE

class QTranslator;

class OptionsDlg : public QDialog
{
	Q_OBJECT

public:
	OptionsDlg(QWidget *parent = nullptr);
	~OptionsDlg();

protected:
	void changeEvent(QEvent*) override;

private slots:
	void on_comboBoxLanguage_currentIndexChanged(int);

private:
	void initComboBoxLanguage();

private:
	Ui::OptionsDlgClass *ui;

	QVector< QTranslator*> m_translators;
};
