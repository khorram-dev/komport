#include <klocalizedstring.h>

/********************************************************************************
** Form generated from reading UI file 'generalpref.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENERALPREF_H
#define UI_GENERALPREF_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "kcolorbutton.h"
#include "kurlrequester.h"

QT_BEGIN_NAMESPACE

class Ui_GeneralPref
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *serialport;
    KUrlRequester *kcfg_serialportDir;
    QHBoxLayout *horizontalLayout_2;
    QLabel *socket;
    KUrlRequester *kcfg_socketDir;
    QFrame *line;
    QCheckBox *kcfg_showSystemTrayIcon;
    QFrame *line_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    KColorButton *kcfg_iconColor;
    QSpacerItem *horizontalSpacer;
    QFrame *line_3;
    QCheckBox *kcfg_socketName;
    QCheckBox *kcfg_newLine;
    QCheckBox *kcfg_timeStamp;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *GeneralPref)
    {
        if (GeneralPref->objectName().isEmpty())
            GeneralPref->setObjectName(QString::fromUtf8("GeneralPref"));
        GeneralPref->resize(485, 401);
        vboxLayout = new QVBoxLayout(GeneralPref);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        groupBox = new QGroupBox(GeneralPref);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        serialport = new QLabel(groupBox);
        serialport->setObjectName(QString::fromUtf8("serialport"));

        horizontalLayout_3->addWidget(serialport);

        kcfg_serialportDir = new KUrlRequester(groupBox);
        kcfg_serialportDir->setObjectName(QString::fromUtf8("kcfg_serialportDir"));

        horizontalLayout_3->addWidget(kcfg_serialportDir);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        socket = new QLabel(groupBox);
        socket->setObjectName(QString::fromUtf8("socket"));

        horizontalLayout_2->addWidget(socket);

        kcfg_socketDir = new KUrlRequester(groupBox);
        kcfg_socketDir->setObjectName(QString::fromUtf8("kcfg_socketDir"));

        horizontalLayout_2->addWidget(kcfg_socketDir);


        verticalLayout->addLayout(horizontalLayout_2);

        line = new QFrame(groupBox);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        kcfg_showSystemTrayIcon = new QCheckBox(groupBox);
        kcfg_showSystemTrayIcon->setObjectName(QString::fromUtf8("kcfg_showSystemTrayIcon"));

        verticalLayout->addWidget(kcfg_showSystemTrayIcon);

        line_2 = new QFrame(groupBox);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        kcfg_iconColor = new KColorButton(groupBox);
        kcfg_iconColor->setObjectName(QString::fromUtf8("kcfg_iconColor"));
        kcfg_iconColor->setAutoFillBackground(false);
        kcfg_iconColor->setAutoDefault(false);
        kcfg_iconColor->setColor(QColor(170, 255, 0));
        kcfg_iconColor->setDefaultColor(QColor(170, 255, 0));

        horizontalLayout->addWidget(kcfg_iconColor);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        line_3 = new QFrame(groupBox);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_3);

        kcfg_socketName = new QCheckBox(groupBox);
        kcfg_socketName->setObjectName(QString::fromUtf8("kcfg_socketName"));
        kcfg_socketName->setChecked(false);

        verticalLayout->addWidget(kcfg_socketName);

        kcfg_newLine = new QCheckBox(groupBox);
        kcfg_newLine->setObjectName(QString::fromUtf8("kcfg_newLine"));
        kcfg_newLine->setChecked(true);

        verticalLayout->addWidget(kcfg_newLine);

        kcfg_timeStamp = new QCheckBox(groupBox);
        kcfg_timeStamp->setObjectName(QString::fromUtf8("kcfg_timeStamp"));
        kcfg_timeStamp->setChecked(false);

        verticalLayout->addWidget(kcfg_timeStamp);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        vboxLayout->addWidget(groupBox);


        retranslateUi(GeneralPref);

        QMetaObject::connectSlotsByName(GeneralPref);
    } // setupUi

    void retranslateUi(QWidget *GeneralPref)
    {
        groupBox->setTitle(tr2i18n("General:", nullptr));
        serialport->setText(tr2i18n("Serialport:", nullptr));
#if QT_CONFIG(tooltip)
        kcfg_serialportDir->setToolTip(tr2i18n("Directory to use as a default save location for all data.", nullptr));
#endif // QT_CONFIG(tooltip)
        socket->setText(tr2i18n("Socket:", nullptr));
#if QT_CONFIG(tooltip)
        kcfg_socketDir->setToolTip(tr2i18n("Directory to use as a default save location for all data.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        kcfg_showSystemTrayIcon->setToolTip(tr2i18n("Whether or not to show system tray popup messages.", nullptr));
#endif // QT_CONFIG(tooltip)
        kcfg_showSystemTrayIcon->setText(tr2i18n("System Tray Icon", nullptr));
        label_2->setText(tr2i18n("Icons Color :", nullptr));
#if QT_CONFIG(tooltip)
        kcfg_socketName->setToolTip(tr2i18n("Whether or not to show system tray popup messages.", nullptr));
#endif // QT_CONFIG(tooltip)
        kcfg_socketName->setText(tr2i18n("Append socket name", nullptr));
#if QT_CONFIG(tooltip)
        kcfg_newLine->setToolTip(tr2i18n("Whether or not to show system tray popup messages.", nullptr));
#endif // QT_CONFIG(tooltip)
        kcfg_newLine->setText(tr2i18n("New Line", nullptr));
#if QT_CONFIG(tooltip)
        kcfg_timeStamp->setToolTip(tr2i18n("Whether or not to show system tray popup messages.", nullptr));
#endif // QT_CONFIG(tooltip)
        kcfg_timeStamp->setText(tr2i18n("Timestamp", nullptr));
        (void)GeneralPref;
    } // retranslateUi

};

namespace Ui {
    class GeneralPref: public Ui_GeneralPref {};
} // namespace Ui

QT_END_NAMESPACE

#endif // GENERALPREF_H

