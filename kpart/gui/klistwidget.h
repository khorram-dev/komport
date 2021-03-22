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

#ifndef KLISTWIDGET_H
#define KLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QScrollBar>
#include <QMessageBox>
#include <QLabel>
#include <libkomport_export.h>

namespace Komport
{
class LIBKOMPORT_EXPORT KListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit KListWidget(QWidget *parent=nullptr);
    ~KListWidget() override;

    void setPath(const QString&);
    void listItemEdit(int id);
    void deleteItem(int);
    void runProfile(int);
    void readItems();

    QPushButton *pb_Edit;
    QPushButton *pb_Delete;
    QPushButton *pb_Run;
    QString path;

private:
    QWidget *wItem;
    QLabel *lName;
    QListWidgetItem *newItem;

    void itemWidgetList(QString name);
    void sendClicked();

Q_SIGNALS:
    void editItem(const QString&);
    void runFile(const QString&);

};
}
#endif // KLISTWIDGET_H
