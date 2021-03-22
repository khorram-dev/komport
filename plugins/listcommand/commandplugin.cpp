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

#include <interfaces/guiinterface.h>
#include <interfaces/serialportinterface.h>
#include <interfaces/socketinterface.h>
#include "commandplugin.h"
#include "guicommand.h"
#include "commandsettings.h"
#include "commandpluginsettings.h"

K_PLUGIN_FACTORY_WITH_JSON(komport_command, "listcommand.json", registerPlugin<Komport::CommandPlugin>();)

namespace Komport {
CommandPlugin::CommandPlugin(QObject* parent, const QVariantList&) : Plugin(parent)
  , lv(nullptr)
  , pref(nullptr)
{
}

CommandPlugin::~CommandPlugin() {}

void CommandPlugin::load()
{
    lv = new guiCommand;
    pref = new commandSettings(nullptr);
    getGUI()->addPrefPage(pref);
    toolview = getGUI()->appendToolView
            (lv, QStringLiteral("commandplugin"), KMultiTabBar::KMultiTabBarPosition::Right,
             Ksvg::seticonSVG(":/images/svg/commandline.svg", Settings::iconColor()), i18n("Command"));
    lv->setParent(toolview);
    connect(lv, &guiCommand::strCommand, this, &CommandPlugin::sendTerminal);
    connect(getGUI(), &GUIInterface::activityChanged, this, &CommandPlugin::setupTab);
}

void CommandPlugin::setupTab()
{
    getGUI()->setVisibleSidebar(toolview, {"Serialport","Socket"});
}

void CommandPlugin::unload()
{
    getGUI()->removePrefPage(pref);
    delete lv;
    lv = nullptr;
    delete pref;
    pref = nullptr;
    delete toolview;
}

void CommandPlugin::sendTerminal(QString text, int time)
{
    if(getGUI()->currentActivity()->name() == "Serialport" && getGUI()->tabIndex() > 0)
        getKSerialport()->sendText(text, time);
    else if(getGUI()->currentActivity()->name() == "Socket" && getGUI()->tabIndex() > 0)
        getSocket()->sendText(text, time);

}

bool CommandPlugin::versionCheck(const QString& version) const
{
    return version == QStringLiteral(KOMPORT_VERSION_MACRO);
}

}
#include "commandplugin.moc"
