#pragma once

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class AboutTinyGISDlgClass; };
QT_END_NAMESPACE

class AboutTinyGISDlg : public QDialog
{
	Q_OBJECT

public:
	AboutTinyGISDlg(QWidget *parent = nullptr);
	~AboutTinyGISDlg();

private:
	Ui::AboutTinyGISDlgClass *ui;
};
