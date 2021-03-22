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
#ifndef KGUICOMMAND_H
#define KGUICOMMAND_H

#include <QDialog>
#include <QVariant>
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QSpinBox>
#include <QMutex>
#include <QDebug>
#include <QMimeDatabase>
#include <QMimeType>

#include <interfaces/activity.h>
#include "ui_guicommand.h"

namespace Komport
{
class guiCommand : public Activity, public Ui::guiCommand
{
    Q_OBJECT
public:
    guiCommand(QWidget* parent = nullptr);
    ~guiCommand() override;
    bool eventFilter(QObject *object, QEvent *event) override;
    void readFiles();

private:
    QListWidgetItem *item;
//    QLabel *laName;
    QLabel *lId;
//    QPushButton *lRun;
//    QPushButton *lEdit;
//    QPushButton *lDelete;
    QWidget *wItem;
    QWidget *widget;
    QDialog *dialogFile;
    QPushButton *pbfAdd;
    QDialog *dialogEdit;
    QLineEdit *fileName;
    QLineEdit *le_Name;
    QSpinBox *sb_Time;
    QTextEdit *te_Description;
    QDialog *catlistDialog;

    QLabel* output;
    QMenu* menu;
    QAction* suspend_action;
    QMutex mutex;
    QStringList pending;

    QMap<int, QStringList> map;

    bool use_rich_text;
    bool suspended;
    int max_block_count;

    void fileDialog();
    void buttonStyle();
    void itemDialog();
    void setItemWidgetList(int row, QString name);

private slots:
    void writeItems(QString mode, int key, QStringList itemList);
    void readItems(const QString&);
    void addFile();
    void deleteFile();
    void renameFile();
    void cbRefresh();

    void slotCommand(int);
    void listItemAdd();
    void listItemEdit(int);
    void listItemDelete(int);
    void sendClicked();

signals:
    void strCommand(QString text, int time);

};

}

#endif
