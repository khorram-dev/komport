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

#include <KToolBar>
#include <QAction>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QStyle>
#include <QTimer>
#include <QMessageBox>
#include <QStandardPaths>
#include <KToolBar>
#include <KLocalizedString>
#include "logviewer.h"
#include "filetree.h"

namespace Komport {
fileTreeView::fileTreeView(LogViewer *logViewer, QWidget* parent) :  Activity(i18n("Log File"), QStringLiteral("view-sidetree"), 100, parent), editor(logViewer)
{
    logFile = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)+QStringLiteral("/log");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    m_toolbar = new KToolBar(this);
    m_toolbar->setMovable(false);
    m_toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_toolbar->setContextMenuPolicy(Qt::NoContextMenu);
    setupActions();
    layout->addWidget(m_toolbar);

    const int iconSize = style()->pixelMetric(QStyle::PM_ButtonIconSize, nullptr, this);
    m_toolbar->setIconSize(QSize(iconSize, iconSize));

    listView = new QListView(this);
    layout->addWidget(listView);
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    listView->setModel(fileModel);

    QDir dir(logFile);
    if(!dir.isEmpty())
        listView->setRootIndex(fileModel->setRootPath(logFile));

    connect(listView,SIGNAL(clicked(const QModelIndex)),this,SLOT(ItemClicked(QModelIndex)));
}

fileTreeView::~fileTreeView(){}

void fileTreeView::setupActions()
{
    actionCollection = new KActionCollection(this);
    auto aReload = actionCollection->addAction(QStringLiteral("file_reload_all"));
    aReload->setText(i18n("Reload Document"));
    aReload->setIcon(QIcon::fromTheme(QStringLiteral("view-refresh")));
    actionCollection->setDefaultShortcut(aReload, Qt::ALT + Qt::Key_Up);
    connect(aReload, &QAction::triggered, this, &fileTreeView::reload);

    auto aDelete = actionCollection->addAction(QStringLiteral("filetree_delete"));
    aDelete->setText(i18n("Delete Current Document"));
    aDelete->setToolTip(i18n("Delete the current document"));
    aDelete->setIcon(QIcon::fromTheme(QStringLiteral("edit-delete-remove")));
    connect(aDelete, &QAction::triggered, this, &fileTreeView::slotDocumentDelete);

    auto aDeleteAll = actionCollection->addAction(QStringLiteral("filetree_delete_all"));
    aDeleteAll->setText(i18n("Delete All Current Documents"));
    aDeleteAll->setToolTip(i18n("Delete All documents under"));
    aDeleteAll->setIcon(QIcon::fromTheme(QStringLiteral("edit-delete")));
    connect(aDeleteAll, &QAction::triggered, this, &fileTreeView::slotDocumentDeleteAll);

    m_toolbar->addAction(aReload);
    m_toolbar->addSeparator();
    m_toolbar->addAction(aDelete);
    m_toolbar->addAction(aDeleteAll);
}

void fileTreeView::ItemClicked (QModelIndex index)
{
    fileIndex = index;
    editor->openUrl(QUrl("file:///"+logFile+"/"+index.data().toString()));
}

void fileTreeView::reload()
{
    listView->setModel(fileModel);
    QDir dir(logFile);
    if(!dir.isEmpty())
    listView->setRootIndex(fileModel->setRootPath(logFile));
}

void fileTreeView::slotDocumentDelete()
{
    QDir file(logFile);
    if(!logFile.isEmpty()) {
        QModelIndex fileIndexNew = fileIndex.siblingAtRow(fileIndex.row()+1);
        file.remove(fileIndex.data().toString());
        fileIndex = fileIndexNew;
    }
}

void fileTreeView::slotDocumentDeleteAll()
{
    QDir file(logFile);
    if(!logFile.isEmpty()) {
        QStringList files = file.entryList();
        for(int i=2; i < files.count(); i++) {
            file.remove(files[i]);
        }
    }
}

}
