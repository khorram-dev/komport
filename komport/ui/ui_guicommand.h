#include <klocalizedstring.h>

/********************************************************************************
** Form generated from reading UI file 'guicommand.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUICOMMAND_H
#define UI_GUICOMMAND_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_guiCommand
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pbFileAdd;
    QPushButton *pbFileRename;
    QPushButton *pbFileDelete;
    QPushButton *pbRefresh;
    QComboBox *cbListFiles;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbItemAdd;
    QListWidget *lwCommand;

    void setupUi(QWidget *guiCommand)
    {
        if (guiCommand->objectName().isEmpty())
            guiCommand->setObjectName(QString::fromUtf8("guiCommand"));
        guiCommand->setWindowModality(Qt::NonModal);
        guiCommand->resize(413, 424);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(guiCommand->sizePolicy().hasHeightForWidth());
        guiCommand->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(guiCommand);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(guiCommand);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pbFileAdd = new QPushButton(guiCommand);
        pbFileAdd->setObjectName(QString::fromUtf8("pbFileAdd"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pbFileAdd->sizePolicy().hasHeightForWidth());
        pbFileAdd->setSizePolicy(sizePolicy1);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../../resource/icons/png/document-edit-128.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbFileAdd->setIcon(icon);
        pbFileAdd->setFlat(true);

        horizontalLayout->addWidget(pbFileAdd);

        pbFileRename = new QPushButton(guiCommand);
        pbFileRename->setObjectName(QString::fromUtf8("pbFileRename"));
        sizePolicy1.setHeightForWidth(pbFileRename->sizePolicy().hasHeightForWidth());
        pbFileRename->setSizePolicy(sizePolicy1);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/Icons/40px/icons8-rename-40.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbFileRename->setIcon(icon1);
        pbFileRename->setFlat(true);

        horizontalLayout->addWidget(pbFileRename);

        pbFileDelete = new QPushButton(guiCommand);
        pbFileDelete->setObjectName(QString::fromUtf8("pbFileDelete"));
        sizePolicy1.setHeightForWidth(pbFileDelete->sizePolicy().hasHeightForWidth());
        pbFileDelete->setSizePolicy(sizePolicy1);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/Icons/40px/icons8-delete-64.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbFileDelete->setIcon(icon2);
        pbFileDelete->setFlat(true);

        horizontalLayout->addWidget(pbFileDelete);

        pbRefresh = new QPushButton(guiCommand);
        pbRefresh->setObjectName(QString::fromUtf8("pbRefresh"));
        sizePolicy1.setHeightForWidth(pbRefresh->sizePolicy().hasHeightForWidth());
        pbRefresh->setSizePolicy(sizePolicy1);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/Icons/40px/icons8-synchronize-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbRefresh->setIcon(icon3);
        pbRefresh->setFlat(true);

        horizontalLayout->addWidget(pbRefresh);


        verticalLayout->addLayout(horizontalLayout);

        cbListFiles = new QComboBox(guiCommand);
        cbListFiles->setObjectName(QString::fromUtf8("cbListFiles"));

        verticalLayout->addWidget(cbListFiles);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(guiCommand);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pbItemAdd = new QPushButton(guiCommand);
        pbItemAdd->setObjectName(QString::fromUtf8("pbItemAdd"));
        sizePolicy1.setHeightForWidth(pbItemAdd->sizePolicy().hasHeightForWidth());
        pbItemAdd->setSizePolicy(sizePolicy1);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/Icons/40px/icons8-plus-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbItemAdd->setIcon(icon4);
        pbItemAdd->setFlat(true);

        horizontalLayout_2->addWidget(pbItemAdd);


        verticalLayout_2->addLayout(horizontalLayout_2);

        lwCommand = new QListWidget(guiCommand);
        lwCommand->setObjectName(QString::fromUtf8("lwCommand"));

        verticalLayout_2->addWidget(lwCommand);


        retranslateUi(guiCommand);

        QMetaObject::connectSlotsByName(guiCommand);
    } // setupUi

    void retranslateUi(QWidget *guiCommand)
    {
        guiCommand->setWindowTitle(tr2i18n("guiCommand", nullptr));
        label_2->setText(tr2i18n("Files:", nullptr));
#if QT_CONFIG(tooltip)
        pbFileAdd->setToolTip(tr2i18n("Add", nullptr));
#endif // QT_CONFIG(tooltip)
        pbFileAdd->setText(tr2i18n("Add", nullptr));
#if QT_CONFIG(tooltip)
        pbFileRename->setToolTip(tr2i18n("Rename", nullptr));
#endif // QT_CONFIG(tooltip)
        pbFileRename->setText(tr2i18n("Rename", nullptr));
#if QT_CONFIG(tooltip)
        pbFileDelete->setToolTip(tr2i18n("Delete", nullptr));
#endif // QT_CONFIG(tooltip)
        pbFileDelete->setText(tr2i18n("Delete", nullptr));
#if QT_CONFIG(tooltip)
        pbRefresh->setToolTip(tr2i18n("Refresh", nullptr));
#endif // QT_CONFIG(tooltip)
        pbRefresh->setText(tr2i18n("Refresh", nullptr));
        label->setText(tr2i18n("Commmands:", nullptr));
        pbItemAdd->setText(tr2i18n("Add", nullptr));
#if QT_CONFIG(tooltip)
        lwCommand->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
    } // retranslateUi

};

namespace Ui {
    class guiCommand: public Ui_guiCommand {};
} // namespace Ui

QT_END_NAMESPACE

#endif // GUICOMMAND_H

