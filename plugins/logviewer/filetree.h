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
#ifndef KATE_FILETREE_PLUGIN_H
#define KATE_FILETREE_PLUGIN_H

#include <QFileSystemModel>
#include <QTreeView>
#include <QListView>
#include <KXMLGUIClient>
#include <interfaces/guiinterface.h>
#include <interfaces/activity.h>

class KToolBar;

class QLineEdit;
namespace Komport {
class LogViewer;
class fileTreeView : public Activity
{
    Q_OBJECT

public:
    fileTreeView(LogViewer* logViewer, QWidget *parent=nullptr);
    ~fileTreeView();

private:
    LogViewer* editor;
    KToolBar *m_toolbar;
    KActionCollection *actionCollection;
    QListView* listView;
    QFileSystemModel *fileModel;
    QModelIndex fileIndex;
    QString logFile;

    void setupActions();


private Q_SLOTS:
    void ItemClicked(QModelIndex index);
    void reload();
    void slotDocumentDelete();
    void slotDocumentDeleteAll();
};
}
#endif // KATE_FILETREE_PLUGIN_H
