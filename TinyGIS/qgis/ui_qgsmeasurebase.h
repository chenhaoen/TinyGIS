/********************************************************************************
** Form generated from reading UI file 'qgsmeasurebase.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QGSMEASUREBASE_H
#define UI_QGSMEASUREBASE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include "qgscollapsiblegroupbox.h"

QT_BEGIN_NAMESPACE

class Ui_QgsMeasureBase
{
public:
    QGridLayout *mGrid;
    QTreeWidget *mTable;
    QLabel *totalDistanceLabel;
    QSpacerItem *spacerItem;
    QLabel *totalHorizontalDistanceLabel;
    QLineEdit *editHorizontalTotal;
    QRadioButton *mEllipsoidal;
    QRadioButton *mCartesian;
    QgsCollapsibleGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QLabel *mNotesLabel;
    QComboBox *mUnitsCombo;
    QDialogButtonBox *buttonBox;
    QLineEdit *editTotal;
    QSpacerItem *mSpacer;

    void setupUi(QDialog *QgsMeasureBase)
    {
        if (QgsMeasureBase->objectName().isEmpty())
            QgsMeasureBase->setObjectName(QString::fromUtf8("QgsMeasureBase"));
        QgsMeasureBase->resize(462, 376);
        QgsMeasureBase->setBaseSize(QSize(150, 200));
        QIcon icon;
        icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        QgsMeasureBase->setWindowIcon(icon);
        mGrid = new QGridLayout(QgsMeasureBase);
        mGrid->setSpacing(6);
        mGrid->setContentsMargins(11, 11, 11, 11);
        mGrid->setObjectName(QString::fromUtf8("mGrid"));
        mGrid->setContentsMargins(9, 9, 9, 9);
        mTable = new QTreeWidget(QgsMeasureBase);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setTextAlignment(0, Qt::AlignTrailing|Qt::AlignVCenter);
        mTable->setHeaderItem(__qtreewidgetitem);
        mTable->setObjectName(QString::fromUtf8("mTable"));
        mTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        mTable->setRootIsDecorated(false);
        mTable->setColumnCount(1);

        mGrid->addWidget(mTable, 2, 0, 1, 4);

        totalDistanceLabel = new QLabel(QgsMeasureBase);
        totalDistanceLabel->setObjectName(QString::fromUtf8("totalDistanceLabel"));

        mGrid->addWidget(totalDistanceLabel, 4, 0, 1, 1);

        spacerItem = new QSpacerItem(41, 25, QSizePolicy::Fixed, QSizePolicy::Minimum);

        mGrid->addItem(spacerItem, 4, 1, 1, 1);

        totalHorizontalDistanceLabel = new QLabel(QgsMeasureBase);
        totalHorizontalDistanceLabel->setObjectName(QString::fromUtf8("totalHorizontalDistanceLabel"));

        mGrid->addWidget(totalHorizontalDistanceLabel, 6, 0, 1, 1);

        editHorizontalTotal = new QLineEdit(QgsMeasureBase);
        editHorizontalTotal->setObjectName(QString::fromUtf8("editHorizontalTotal"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        editHorizontalTotal->setFont(font);
        editHorizontalTotal->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        editHorizontalTotal->setReadOnly(true);

        mGrid->addWidget(editHorizontalTotal, 6, 2, 1, 1);

        mEllipsoidal = new QRadioButton(QgsMeasureBase);
        mEllipsoidal->setObjectName(QString::fromUtf8("mEllipsoidal"));
        mEllipsoidal->setChecked(true);

        mGrid->addWidget(mEllipsoidal, 7, 2, 1, 1);

        mCartesian = new QRadioButton(QgsMeasureBase);
        mCartesian->setObjectName(QString::fromUtf8("mCartesian"));
        mCartesian->setChecked(false);

        mGrid->addWidget(mCartesian, 7, 0, 1, 1);

        groupBox = new QgsCollapsibleGroupBox(QgsMeasureBase);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        mNotesLabel = new QLabel(groupBox);
        mNotesLabel->setObjectName(QString::fromUtf8("mNotesLabel"));
        mNotesLabel->setWordWrap(true);

        verticalLayout->addWidget(mNotesLabel);


        mGrid->addWidget(groupBox, 9, 0, 1, 4);

        mUnitsCombo = new QComboBox(QgsMeasureBase);
        mUnitsCombo->setObjectName(QString::fromUtf8("mUnitsCombo"));

        mGrid->addWidget(mUnitsCombo, 4, 3, 1, 1);

        buttonBox = new QDialogButtonBox(QgsMeasureBase);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Close|QDialogButtonBox::Help);

        mGrid->addWidget(buttonBox, 11, 0, 1, 4);

        editTotal = new QLineEdit(QgsMeasureBase);
        editTotal->setObjectName(QString::fromUtf8("editTotal"));
        editTotal->setFont(font);
        editTotal->setAlignment(Qt::AlignRight);
        editTotal->setReadOnly(true);

        mGrid->addWidget(editTotal, 4, 2, 1, 1);

        mSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        mGrid->addItem(mSpacer, 10, 0, 1, 4);

#if QT_CONFIG(shortcut)
        totalDistanceLabel->setBuddy(editTotal);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(mTable, editTotal);

        retranslateUi(QgsMeasureBase);

        QMetaObject::connectSlotsByName(QgsMeasureBase);
    } // setupUi

    void retranslateUi(QDialog *QgsMeasureBase)
    {
        QgsMeasureBase->setWindowTitle(QCoreApplication::translate("QgsMeasureBase", "Measure", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = mTable->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("QgsMeasureBase", "Segments", nullptr));
        totalDistanceLabel->setText(QCoreApplication::translate("QgsMeasureBase", "Total", nullptr));
        totalHorizontalDistanceLabel->setText(QCoreApplication::translate("QgsMeasureBase", "Total Horizontal Distance", nullptr));
        mEllipsoidal->setText(QCoreApplication::translate("QgsMeasureBase", "Ellipsoidal", nullptr));
        mCartesian->setText(QCoreApplication::translate("QgsMeasureBase", "Cartesian", nullptr));
        groupBox->setTitle(QCoreApplication::translate("QgsMeasureBase", "Info", nullptr));
        mNotesLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QgsMeasureBase: public Ui_QgsMeasureBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QGSMEASUREBASE_H
