/***************************************************************************
 *   Copyright (C) 2019 by Masoud Khorram                                  *
 *   masoudkh62@gmail.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "klistwidget.h"
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QMenu>
#include <QVBoxLayout>
#include <QPushButton>
#include <settings.h>
#include "util/ksvg.h"

namespace Komport
{
KListWidget::KListWidget(QWidget *parent) : QListWidget(parent), path(QDir::homePath())
{
    setContentsMargins(0,0,0,0);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setAcceptDrops(true);
    readItems();
}

KListWidget::~KListWidget() {}

void KListWidget::setPath(const QString& text)
{
    path = text;
}

void KListWidget::itemWidgetList(QString name)
{
    wItem = new QWidget(this);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(wItem->sizePolicy().hasHeightForWidth());
    wItem->setSizePolicy(sizePolicy);
    QVBoxLayout *verticalLayout = new QVBoxLayout(wItem);
    QHBoxLayout *horizontalLayout = new QHBoxLayout();

    lName = new QLabel(wItem);
    //    lName->setSizePolicy(sizePolicy);
    lName->setObjectName(QString::fromUtf8("lName"));
    lName->setText(name);

    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QSizePolicy pbSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pbSizePolicy.setHorizontalStretch(0);
    pbSizePolicy.setVerticalStretch(0);
//    pbSizePolicy.setHeightForWidth(pb_Edit->sizePolicy().hasHeightForWidth());
//    pb_Edit->setSizePolicy(pbSizePolicy);

    pb_Edit = new QPushButton(wItem);
    pb_Edit->setProperty("itemEdit", row(newItem));
    pb_Edit->setToolTip("Edit");
    pb_Edit->setObjectName(QString::number(row(newItem)));
    pb_Edit->setText(QApplication::translate("wItem", "", nullptr));
    pb_Edit->setIcon(Ksvg::seticonSVG(":/images/svg/document-edit.svg", Settings::iconColor()));
    pb_Edit->setFlat(true);

    pb_Delete = new QPushButton(wItem);
    pb_Delete->setProperty("itemDelete", row(newItem));
    pb_Delete->setToolTip("Delete");
    pb_Delete->setObjectName(QString::number(row(newItem)));
    pb_Delete->setText(QApplication::translate("wItem", "", nullptr));
    pb_Delete->setIcon(Ksvg::seticonSVG(":/images/svg/document-delete.svg", Settings::iconColor()));
    pb_Delete->setFlat(true);

    pb_Run = new QPushButton(wItem);
    pb_Run->setProperty("itemRun", row(newItem));
    pb_Run->setToolTip("Run");
    //    pb_Run->setSizePolicy(sizePolicy);
    pb_Run->setObjectName(QString::number(row(newItem)));
    pb_Run->setText(QApplication::translate("wItem", "", nullptr));
    pb_Run->setIcon(Ksvg::seticonSVG(":/images/svg/start.svg", Settings::iconColor()));
    pb_Run->setFlat(true);

    pbSizePolicy.setHeightForWidth(pb_Run->sizePolicy().hasHeightForWidth());
    pb_Run->setSizePolicy(pbSizePolicy);

    horizontalLayout->addWidget(lName);
    horizontalLayout->addItem(horizontalSpacer);
    horizontalLayout->addWidget(pb_Edit);
    horizontalLayout->addWidget(pb_Delete);
    horizontalLayout->addWidget(pb_Run);
    verticalLayout->addLayout(horizontalLayout);

    QMetaObject::connectSlotsByName(wItem);

    connect(pb_Edit, &QPushButton::clicked, this, &KListWidget::sendClicked);
    connect(pb_Delete, &QPushButton::clicked, this, &KListWidget::sendClicked);
    connect(pb_Run, &QPushButton::clicked, this, &KListWidget::sendClicked);
}

void KListWidget::sendClicked()
{
    QPushButton* button = static_cast<QPushButton*>(sender());
    if(button->property("itemRun").isValid())
    {
        int digit = button->property("itemRun").toInt();
        runProfile(digit);
    } else if(button->property("itemEdit").isValid()) {
        int digit = button->property("itemEdit").toInt();
        listItemEdit(digit);
    } else if(button->property("itemDelete").isValid()) {
        int digit = button->property("itemDelete").toInt();
        deleteItem(digit);
    }
}

void KListWidget::listItemEdit(int id)
{
    setCurrentRow(id);
    QWidget *itemList = qobject_cast<QWidget *>(itemWidget(item(id)));
    Q_EMIT editItem(itemList->findChild<QLabel*>("lName")->text());
}

void KListWidget::deleteItem(int id)
{
    QMessageBox msgBox;
    msgBox.setText("The item has been deleted.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
        case QMessageBox::Ok:
        {        
            setCurrentRow(id);
            QWidget *itemList;
            itemList = qobject_cast<QWidget *>(itemWidget(item(id)));
            QString textName = itemList->findChild<QLabel*>("lName")->text();
            QFile file(path+"/"+textName);
            file.remove();
            readItems();
            break;
        }
        case QMessageBox::Cancel:
            // Cancel was clicked
            break;
      }
}

void KListWidget::runProfile(int id)
{
    setCurrentRow(id);
    QWidget *itemList = qobject_cast<QWidget *>(itemWidget(item(id)));
    Q_EMIT runFile(path+"/"+itemList->findChild<QLabel*>("lName")->text());
}

void KListWidget::readItems()
{
    clear();
    QDir pathFiles(path);
    if(!pathFiles.isEmpty())
    {
        QStringList files = pathFiles.entryList(QDir::Files);
        foreach(QString file, files)
        {
            newItem = new QListWidgetItem(this);
            itemWidgetList(file);
            newItem->setSizeHint(wItem->sizeHint());
            setItemWidget(newItem, wItem);
        }
    }
}
}
