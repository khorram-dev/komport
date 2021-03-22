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

#ifndef KTPLUGINMANAGER_H
#define KTPLUGINMANAGER_H

#include <QList>
#include <QStringList>
#include <QVector>

#include <KPluginInfo>
#include <KPluginMetaData>

#include <util/ptrmap.h>
#include <interfaces/plugin.h>
#include <libkomport_export.h>

namespace Komport
{
class GUIInterface;
class SerialportInterface;
class SocketInterface;
class PluginSetting;

/**
* @author Masoud Khorram
* @brief Class to manage plugins
*
* This class manages all plugins. Plugins are stored in a map
*/
class LIBKOMPORT_EXPORT PluginManager
{
    KPluginInfo::List plugins;
    QVector<KPluginMetaData> pluginsMetaData;
    GUIInterface* gui;
    SerialportInterface* serialport;
    SocketInterface* socket;
    PluginSetting* pluginSetting;
    Komport::PtrMap<int, Plugin> loaded;

public:
    PluginManager(GUIInterface* gui, SerialportInterface* serialport, SocketInterface* socket);
    ~PluginManager();


    /**
    * Get the plugin info list.
    */
    const KPluginInfo::List& pluginInfoList() const {return plugins;}

    /**
    * Load the list of plugins.
    * This basically uses KTrader to get a list of available plugins, and
    * loads those, but does not initialize them. We will consider a plugin loaded
    * when it's load method is called.
    */
    void loadPluginList();

    /**
    * Check the PluginInfo of each plugin and unload or load it if necessary
    */
    void loadPlugins();

    /**
    * Update all plugins who need a periodical GUI update.
    */
    void updateGuiPlugins();

    /**
    * Unload all plugins.
    */
    void unloadAll();

private:
    void load(const KPluginInfo& pi, int idx);
    void unload(const KPluginInfo& pi, int idx);
};

}

#endif
