#include <klocalizedstring.h>

/********************************************************************************
** Form generated from reading UI file 'commandsettings.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMANDSETTINGS_H
#define UI_COMMANDSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "kurlrequester.h"

QT_BEGIN_NAMESPACE

class Ui_commandSettings
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    KUrlRequester *kcfg_saveDir;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *kcfg_listview;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *commandSettings)
    {
        if (commandSettings->objectName().isEmpty())
            commandSettings->setObjectName(QString::fromUtf8("commandSettings"));
        commandSettings->resize(380, 228);
        verticalLayout = new QVBoxLayout(commandSettings);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(commandSettings);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        kcfg_saveDir = new KUrlRequester(commandSettings);
        kcfg_saveDir->setObjectName(QString::fromUtf8("kcfg_saveDir"));

        horizontalLayout_2->addWidget(kcfg_saveDir);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(commandSettings);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        kcfg_listview = new QComboBox(commandSettings);
        kcfg_listview->addItem(QString());
        kcfg_listview->addItem(QString());
        kcfg_listview->setObjectName(QString::fromUtf8("kcfg_listview"));

        horizontalLayout->addWidget(kcfg_listview);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(commandSettings);

        QMetaObject::connectSlotsByName(commandSettings);
    } // setupUi

    void retranslateUi(QWidget *commandSettings)
    {
        label_2->setText(tr2i18n("Path files:", nullptr));
#if QT_CONFIG(tooltip)
        kcfg_saveDir->setToolTip(tr2i18n("Directory to use as a default save location for all data.", nullptr));
#endif // QT_CONFIG(tooltip)
        label->setText(tr2i18n("View Lable:", nullptr));
        kcfg_listview->setItemText(0, tr2i18n("Command", nullptr));
        kcfg_listview->setItemText(1, tr2i18n("Description", nullptr));

        (void)commandSettings;
    } // retranslateUi

};

namespace Ui {
    class commandSettings: public Ui_commandSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // COMMANDSETTINGS_H

