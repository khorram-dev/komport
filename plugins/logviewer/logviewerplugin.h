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
#ifndef KTLOGVIEWERPLUGIN_H
#define KTLOGVIEWERPLUGIN_H

#include <interfaces/plugin.h>
#include <interfaces/activity.h>
#include <interfaces/guiinterface.h>

namespace KTextEditor
{
class Document;
class View;
}


namespace Komport
{
class LogViewer;
class LogViewerSettings;
class fileTreeView;

class LogViewerPlugin : public Plugin
{
    Q_OBJECT
public:
    LogViewerPlugin(QObject* parent, const QVariantList& args);
    ~LogViewerPlugin() override;

    void load() override;
    void unload() override;
    bool versionCheck(const QString& version) const override;
//    void guiUpdate() override;

public Q_SLOT:

private:
//    void activityChanged();
    void iconColor();
    ActivityInterface *activeInterface;
    LogViewer* lv;
    LogViewerSettings* pref;
    fileTreeView* treeView;

    KTextEditor::View *m_view;
    KTextEditor::Document *m_doc;

    QWidget *toolview;

private Q_SLOT:
    void activityChanged();
};

}

#endif
