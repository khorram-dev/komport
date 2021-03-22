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
#include <KPluginFactory>
#include <KLocalizedString>
#include <KMainWindow>
#include <QStatusBar>

#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QScreen>
#include <QScroller>
#include <QTreeView>

#include "logviewerplugin.h"
#include "logviewer.h"
#include "filetree.h"

K_PLUGIN_FACTORY_WITH_JSON(komport_logviewer, "logviewer.json", registerPlugin<Komport::LogViewerPlugin>();)

namespace Komport {
LogViewerPlugin::LogViewerPlugin(QObject* parent, const QVariantList&) : Plugin(parent)
  , lv(nullptr)
  , pref(nullptr)
{
}

LogViewerPlugin::~LogViewerPlugin(){}

void LogViewerPlugin::load()
{
    lv = new LogViewer(getGUI()->iconColor());
//    connect(getGUI(), &GUIInterface::settingsChanged, lv, &LogViewer::settingsChanged);
    getGUI()->addActivity(lv);
    treeView = new fileTreeView(lv);
    toolview = getGUI()->appendToolView
            (treeView, QStringLiteral("logfileplugin"), KMultiTabBar::KMultiTabBarPosition::Right,
             Ksvg::seticonSVG(":/images/svg/commandline.svg", getGUI()->iconColor()), i18n("Log File"));
    treeView->setParent(toolview);

    connect(getGUI(), &GUIInterface::activityChanged, this, &LogViewerPlugin::activityChanged);
}

void LogViewerPlugin::unload()
{
    disconnect(getGUI(), &GUIInterface::activityChanged, this, &LogViewerPlugin::activityChanged);
    getGUI()->removeActivity(lv);
    delete lv;
    lv = nullptr;
    pref = nullptr;
}

bool LogViewerPlugin::versionCheck(const QString& version) const
{
    return version == QStringLiteral(KOMPORT_VERSION_MACRO);
}

void LogViewerPlugin::activityChanged()
{
    if(lv->isVisible()) {
        getGUI()->setToolView(toolview);
    }
}

void LogViewerPlugin::iconColor()
{
    getGUI()->iconColor();
}

}
#include "logviewerplugin.moc"
