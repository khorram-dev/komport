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

#include <QApplication>
#include <QBoxLayout>
#include <QIcon>
#include <QMenu>
#include <QStringList>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QMimeData>
#include <QDrag>
#include <QFile>
#include <QDir>
#include <QKeyEvent>
#include <QModelIndex>
#include <QMessageBox>

#include <KConfig>
#include <KLocalizedString>
#include <KMultiTabBar>
#include "commandpluginsettings.h"
#include "guicommand.h"

namespace Komport {

guiCommand::guiCommand(QWidget* parent) : Activity(i18n("List Command"), QStringLiteral("view-sidetree"), 100, parent)
{
    setToolTip(i18n("View the List Command by Komport"));
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    QWidget *wdg = new QWidget(this);
    layout->addWidget(wdg);
    setupUi(wdg);
    lwCommand->setCurrentRow(0);

    fileDialog();
    readFiles();
    buttonStyle();
    itemDialog();

    cbListFiles->setCurrentIndex(0);
    if(cbListFiles->currentIndex() > 0)
    readItems(cbListFiles->currentText());

    pbFileAdd->setIcon(QIcon::fromTheme(QStringLiteral("window-new")));
    pbFileRename->setIcon(QIcon::fromTheme(QStringLiteral("edit-rename")));
    pbFileDelete->setIcon(QIcon::fromTheme(QStringLiteral("edit-delete")));
    pbRefresh->setIcon(QIcon::fromTheme(QStringLiteral("view-refresh")));
    pbItemAdd->setIcon(QIcon::fromTheme(QStringLiteral("window-new")));

    connect(pbItemAdd, &QPushButton::clicked, this, &guiCommand::listItemAdd);
    connect(pbFileAdd, &QPushButton::clicked, this, &guiCommand::addFile);
    connect(pbFileDelete, &QPushButton::clicked, this, &guiCommand::deleteFile);
    connect(pbFileRename, &QPushButton::clicked, this, &guiCommand::renameFile);
    connect(pbRefresh, &QPushButton::clicked, this, &guiCommand::cbRefresh);
    connect(cbListFiles, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(readItems(const QString&)));
    cbRefresh();
}

guiCommand::~guiCommand(){}

bool guiCommand::eventFilter(QObject *object, QEvent *event)
{
    if (object == lwCommand && event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Escape)
        {
           // special Esc handling here
            qDebug() << "Click Escape";
            return true;
        } else {
            return false;
        }
    }
    return true;
}

void guiCommand::buttonStyle()
{
    pbItemAdd->setText(nullptr);
    pbFileAdd->setText(nullptr);
    pbFileDelete->setText(nullptr);
    pbFileRename->setText(nullptr);
    pbRefresh->setText(nullptr);
}

// Items ListWidget
void guiCommand::setItemWidgetList(int row, QString name)
{
    wItem = new QWidget;
    if (wItem->objectName().isEmpty())
        wItem->setObjectName(QString::fromUtf8("wItem"));
    wItem->resize(300, 46);
    QVBoxLayout *verticalLayout = new QVBoxLayout;
    wItem->setLayout(verticalLayout);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(wItem->sizePolicy().hasHeightForWidth());
    wItem->setSizePolicy(sizePolicy);
    QHBoxLayout *horizontalLayout = new QHBoxLayout(wItem);
    QLabel *lb_Row = new QLabel(wItem);
    lb_Row->setObjectName(QString::fromUtf8("lb_Row"));
    lb_Row->setText(QString::number(row));
    lb_Row->hide();

    horizontalLayout->setSizeConstraint(QLayout::SetMinimumSize);

    QLabel *lb_Name = new QLabel(wItem);
    lb_Name->setObjectName(QString::fromUtf8("lb_Name"));
    QString rname;
    if(name.length() > commandPluginSettings::lineedit()) {
        rname = name.remove(commandPluginSettings::lineedit(), name.length());
        rname.append("...");
        lb_Name->setText(rname);
    } else {
        lb_Name->setText(name);
    }

    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QPushButton *pb_Edit = new QPushButton(wItem);
    pb_Edit->setProperty("itemEdit", lwCommand->row(item));
    pb_Edit->setToolTip("Edit");
    pb_Edit->setObjectName(QString::number(lwCommand->row(item)));
    pb_Edit->setText(QApplication::translate("wItem", "", nullptr));
    pb_Edit->setIcon(QIcon::fromTheme(QStringLiteral("edit-rename")));
    pb_Edit->setFlat(true);
    QSizePolicy pbSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pbSizePolicy.setHorizontalStretch(0);
    pbSizePolicy.setVerticalStretch(0);
    pbSizePolicy.setHeightForWidth(pb_Edit->sizePolicy().hasHeightForWidth());
    pb_Edit->setSizePolicy(pbSizePolicy);
    QPushButton *pb_Delete = new QPushButton(wItem);
    pb_Delete->setProperty("itemDelete", lwCommand->row(item));
    pb_Delete->setToolTip("Delete");
    pb_Delete->setObjectName(QString::number(lwCommand->row(item)));
    pb_Delete->setText(QApplication::translate("wItem", "", nullptr));
    pb_Delete->setIcon(QIcon::fromTheme(QStringLiteral("edit-delete")));
    pb_Delete->setFlat(true);
    QPushButton *pb_Run = new QPushButton(wItem);
    pb_Run->setProperty("itemRun", lwCommand->row(item));
    pb_Run->setToolTip("Run");
//    pb_Run->setSizePolicy(sizePolicy);
    pb_Run->setObjectName(QString::number(lwCommand->row(item)));
    pb_Run->setText(QApplication::translate("wItem", "", nullptr));
    pb_Run->setIcon(QIcon::fromTheme(QStringLiteral("system-run")));
    pb_Run->setFlat(true);
    pbSizePolicy.setHeightForWidth(pb_Run->sizePolicy().hasHeightForWidth());
    pb_Run->setSizePolicy(pbSizePolicy);

    horizontalLayout->addWidget(lb_Name);
    horizontalLayout->addItem(horizontalSpacer);
    horizontalLayout->addWidget(pb_Edit);
    horizontalLayout->addWidget(pb_Delete);
    horizontalLayout->addWidget(pb_Run);
    verticalLayout->addLayout(horizontalLayout);
    QMetaObject::connectSlotsByName(wItem);

    connect(pb_Edit, &QPushButton::clicked, this, &guiCommand::sendClicked);
    connect(pb_Delete, &QPushButton::clicked, this, &guiCommand::sendClicked);
    connect(pb_Run, &QPushButton::clicked, this, &guiCommand::sendClicked);
}

void guiCommand::sendClicked()
{
    QPushButton* button = static_cast<QPushButton*>(sender());
    if(button->property("itemRun").isValid())
    {
        int digit = button->property("itemRun").toInt();
        slotCommand(digit);
    } else if(button->property("itemEdit").isValid()) {
        int digit = button->property("itemEdit").toInt();
        listItemEdit(digit);
    } else if(button->property("itemDelete").isValid()) {
        int digit = button->property("itemDelete").toInt();
        listItemDelete(digit);
    }
}

void guiCommand::slotCommand(int id)
{
    lwCommand->setCurrentRow(id);
    QWidget *witem = qobject_cast<QWidget *>(lwCommand->itemWidget(lwCommand->item(id)));
    int row = witem->findChild<QLabel*>("lb_Row")->text().toInt();
    QStringList itemMap = map.value(row);
    Q_EMIT strCommand(itemMap[0], itemMap[1].toInt());
}

void guiCommand::listItemAdd()
{
    if (dialogEdit->exec() == dialogEdit->Accepted) {
        item = new QListWidgetItem(lwCommand);
        setItemWidgetList(lwCommand->row(item), le_Name->text());
        item->setSizeHint(wItem->sizeHint());
        lwCommand->setItemWidget(item, wItem);
        QStringList itemList = {le_Name->text(), sb_Time->text(), te_Description->toPlainText()};
        writeItems("New", 0, itemList);
    }
}

void guiCommand::listItemEdit(int id)
{
    lwCommand->setCurrentRow(id);
    QWidget *witem = qobject_cast<QWidget *>(lwCommand->itemWidget(lwCommand->item(id)));
    int row = witem->findChild<QLabel*>("lb_Row")->text().toInt();
    QStringList itemMap = map.value(row);
    le_Name->setText(itemMap[0]);
    sb_Time->setValue(itemMap[1].toInt());
    te_Description->setText(itemMap[2]);
    if (dialogEdit->exec() == dialogEdit->Accepted) {
        QStringList itemList = {le_Name->text(), sb_Time->text(), te_Description->toPlainText()};
        writeItems("Edit", row, itemList);
    }
}

void guiCommand::listItemDelete(int id)
{
    QMessageBox msgBox;
    msgBox.setText("The item has been deleted.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    lwCommand->setCurrentRow(id);
    QWidget *itemList = qobject_cast <QWidget *>(lwCommand->itemWidget(lwCommand->item(id)));
    int row = itemList->findChild<QLabel*>("lb_Row")->text().toInt();
    switch (ret) {
        case QMessageBox::Yes:
            delete lwCommand->currentItem();
            writeItems("Delete", row, {});
            break;
        case QMessageBox::Cancel:
            // Cancel was clicked
            break;
      }
}

void guiCommand::itemDialog()
{
    dialogEdit = new QDialog;
    if (dialogEdit->objectName().isEmpty())
        dialogEdit->setObjectName(QString::fromUtf8("dialogEdit"));
    dialogEdit->resize(300, 100);
    QVBoxLayout *vb_Dialog = new QVBoxLayout;
    vb_Dialog->setObjectName("vb_Dialog");
//    vb_Dialog->setParent(dialogEdit);
    dialogEdit->setLayout(vb_Dialog);
    QVBoxLayout *vb_Name = new QVBoxLayout;
    vb_Name->setObjectName(QString::fromUtf8("vb_Name"));
    QLabel *lbName = new QLabel;
    lbName->setObjectName(QString::fromUtf8("lb_Name"));
    lbName->setText(QApplication::translate("dialogEdit", "Command:", nullptr));
    le_Name = new QLineEdit;
    le_Name->setObjectName(QString::fromUtf8("le_Name"));
    vb_Name->addWidget(lbName);
    vb_Name->addWidget(le_Name);

    QVBoxLayout *vb_Time = new QVBoxLayout;
    vb_Time->setObjectName(QString::fromUtf8("vb_Time"));
    QLabel *lbTime = new QLabel;
    lbTime->setObjectName(QString::fromUtf8("lb_Time"));
    lbTime->setText(QApplication::translate("dialogEdit", "Time:", nullptr));
    sb_Time = new QSpinBox;
    sb_Time->setObjectName(QString::fromUtf8("sb_Time"));
    sb_Time->setRange(0,999999);
    sb_Time->setValue(1000);
    vb_Time->addWidget(lbTime);
    vb_Time->addWidget(sb_Time);

    QVBoxLayout *vbDescription = new QVBoxLayout;
    vbDescription->setObjectName(QString::fromUtf8("vb_Description"));
    QLabel *lbDescription = new QLabel;
    lbDescription->setObjectName(QString::fromUtf8("lb_Descripton"));
    lbDescription->setText(QApplication::translate("dialogEdit", "Description:", nullptr));
    te_Description = new QTextEdit;
    te_Description->setObjectName(QString::fromUtf8("leDescription"));
    vbDescription->addWidget(lbDescription);
    vbDescription->addWidget(te_Description);

    QHBoxLayout *hbEnd = new QHBoxLayout;
    hbEnd->setObjectName(QString::fromUtf8("hb_End"));
    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hbEnd->addItem(horizontalSpacer);
    QPushButton *pbItemEdit = new QPushButton;
    pbItemEdit->setObjectName(QString::fromUtf8("pbItemEdit"));
    QPushButton *pbItemCancel = new QPushButton;
    pbItemCancel->setObjectName(QString::fromUtf8("pbItemCancel"));
    hbEnd->addWidget(pbItemEdit);
    hbEnd->addWidget(pbItemCancel);

    vb_Dialog->addLayout(vb_Name);
    vb_Dialog->addLayout(vb_Time);
    vb_Dialog->addLayout(vbDescription);
    vb_Dialog->addLayout(hbEnd);

    QMetaObject::connectSlotsByName(dialogEdit);

    dialogEdit->setWindowTitle(QApplication::translate("dialogEdit", "Edit Item", nullptr));
    pbItemEdit->setText(QApplication::translate("dialogEdit", "Ok", nullptr));
    pbItemCancel->setText(QApplication::translate("dialogEdit", "Cancel", nullptr));

    connect(pbItemCancel, &QPushButton::clicked, dialogEdit, &QDialog::close);
    connect(pbItemEdit, &QPushButton::clicked, dialogEdit, &QDialog::accept);
}

void guiCommand::writeItems(QString mode, int key, QStringList itemList)
{
    QFile file(commandPluginSettings::saveDir()+"/"+cbListFiles->currentText());
    if (!file.open(QFile::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "File not open!");
        return;
    }
    int number=0;
    QMapIterator<int, QStringList> i(map);
    while (i.hasNext()) {
        i.next();
        if(number < i.key())
            number = i.key();
    }

    if(mode == "New")
        map.insert(number+1, itemList);
    else if(mode == "Edit")
        map[key] = itemList;
    else if(mode == "Delete")
        map.remove(key);

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_1);
    out << map;
    file.flush();
    file.close();
    readItems(cbListFiles->currentText());
}

void guiCommand::readItems(const QString &lFiles)
{
    map.clear();
    lwCommand->clear();
    QFile file(commandPluginSettings::saveDir()+"/"+lFiles);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(commandPluginSettings::saveDir()+"/"+lFiles, QMimeDatabase::MatchContent);
    if(mime.name() == "application/octet-stream") {
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_1);
        in >> map;
        file.close();

        int lview = 0;
        if(commandPluginSettings::listview() > 0)
            lview = 2;

        QMapIterator<int, QStringList> i(map);
        QStringList itemList;
        while (i.hasNext()) {
            i.next();
            itemList = map[i.key()];
            if(!itemList.isEmpty()) {
                item = new QListWidgetItem(lwCommand);
                setItemWidgetList(i.key(), itemList.at(lview).toLocal8Bit().constData());
                item->setSizeHint(wItem->sizeHint());
                lwCommand->setItemWidget(item, wItem);
                wItem->setToolTip("Command:\n"+QString(itemList.at(0).toLocal8Bit().constData())+"\n\nDescription: "+QString(itemList.at(2).toLocal8Bit().constData()));
            }
            itemList.clear();
        }
    }
    file.close();
}

void guiCommand::readFiles()
{
    QDir path(commandPluginSettings::saveDir());
    QStringList files = path.entryList(QDir::Files);
    if(files.isEmpty()) {
        pbItemAdd->setDisabled(true);
        cbListFiles->setDisabled(true);
    } else {
        pbItemAdd->setDisabled(false);
        cbListFiles->setDisabled(false);
        cbListFiles->addItems(files);
    }
}

void guiCommand::fileDialog()
{
    dialogFile = new QDialog;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbfAdd;
    QPushButton *pbfCancel;

    if (dialogFile->objectName().isEmpty())
        dialogFile->setObjectName(QString::fromUtf8("dialogFile"));
    dialogFile->resize(300, 100);
    verticalLayout = new QVBoxLayout(dialogFile);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    label = new QLabel(dialogFile);
    label->setObjectName(QString::fromUtf8("label"));
    verticalLayout->addWidget(label);

    fileName = new QLineEdit(dialogFile);
    fileName->setObjectName(QString::fromUtf8("fileName"));
    verticalLayout->addWidget(fileName);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);

    pbfAdd = new QPushButton(dialogFile);
    pbfAdd->setObjectName(QString::fromUtf8("pbfAdd"));
    horizontalLayout->addWidget(pbfAdd);

    pbfCancel = new QPushButton(dialogFile);
    pbfCancel->setObjectName(QString::fromUtf8("pbfCancel"));
    horizontalLayout->addWidget(pbfCancel);
    verticalLayout->addLayout(horizontalLayout);

    QMetaObject::connectSlotsByName(dialogFile);

    dialogFile->setWindowTitle(QApplication::translate("dialogFile", "Add File", nullptr));
    label->setText(QApplication::translate("dialogFile", "Name File:", nullptr));
    pbfAdd->setText(QApplication::translate("dialogFile", "Submit", nullptr));
    pbfCancel->setText(QApplication::translate("dialogFile", "Cancel", nullptr));

    connect(pbfCancel, &QPushButton::clicked, dialogFile, &QDialog::close);
    connect(pbfAdd, &QPushButton::clicked, dialogFile, &QDialog::accept);
}

void guiCommand::addFile()
{
    if(dialogFile->exec() == dialogFile->Accepted)
    {
        QDir dir(commandPluginSettings::saveDir());
        if(!dir.exists())
            dir.mkdir(commandPluginSettings::saveDir());

        QFile file(commandPluginSettings::saveDir()+"/"+fileName->text());
        if(!file.exists())
        {
            file.open(QIODevice::WriteOnly);
            cbRefresh();
        }
    }
}

void guiCommand::renameFile()
{
    dialogFile->setWindowTitle(QApplication::translate("dialogFile", "Rename File", nullptr));
    fileName->setText(cbListFiles->currentText());
    if(dialogFile->exec() == dialogFile->Accepted)
    {
        QFile file(commandPluginSettings::saveDir()+"/"+cbListFiles->currentText());
        if(file.exists())
        {
            file.rename(commandPluginSettings::saveDir()+"/"+fileName->text());
            cbRefresh();
        } else {
            QMessageBox::warning(this, "Error", "File not exist!!!");
        }
    }
}

void guiCommand::deleteFile()
{
    QMessageBox msgBox;
    msgBox.setText("The file has been deleted.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();

    switch (ret) {
        case QMessageBox::Ok: {
            QFile file(commandPluginSettings::saveDir()+"/"+cbListFiles->currentText());
            file.remove();
            cbListFiles->removeItem(cbListFiles->currentIndex());
            }
            break;
        case QMessageBox::Cancel:
            // Cancel was clicked
            break;
      }
    cbRefresh();
}

void guiCommand::cbRefresh()
{
    cbListFiles->clear();
    QDir path;
    path.setPath(commandPluginSettings::saveDir());
    QStringList files = path.entryList(QDir::Files);
    if(files.isEmpty()) {
        pbItemAdd->setDisabled(true);
        cbListFiles->setDisabled(true);
    } else {
        pbItemAdd->setDisabled(false);
        cbListFiles->setDisabled(false);
        cbListFiles->addItems(files);
    }
}

}
