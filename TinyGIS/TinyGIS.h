#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TinyGIS.h"

class TinyGIS : public QMainWindow
{
    Q_OBJECT

public:
    TinyGIS(QWidget *parent = Q_NULLPTR);

private:
    Ui::TinyGISClass ui;
};
