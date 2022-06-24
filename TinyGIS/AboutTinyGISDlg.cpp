#include "AboutTinyGISDlg.h"

#include <qgsconfig.h>

AboutTinyGISDlg::AboutTinyGISDlg(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::AboutTinyGISDlgClass())
{
	ui->setupUi(this);

	ui->label_qgis_version->setText(VERSION);
	ui->label_qt_version->setText(qVersion());
}

AboutTinyGISDlg::~AboutTinyGISDlg()
{
	delete ui;
}
