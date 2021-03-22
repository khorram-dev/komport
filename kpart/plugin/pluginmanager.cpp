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

#include "pluginmanager.h"

#include <QFile>
#include <QTextStream>

#include <KLocalizedString>
#include <KSharedConfig>
#include <KPluginMetaData>

#include <util/log.h>
#include <util/error.h>
#include <util/waitjob.h>
#include <interfaces/guiinterface.h>
#include <interfaces/serialportinterface.h>
#include <interfaces/socketinterface.h>
#include "pluginsetting.h"

#include <QDebug>

namespace Komport
{

PluginManager::PluginManager(GUIInterface* gui, SerialportInterface* serialport, SocketInterface* socket) : gui(gui), serialport(serialport), socket(socket)
{
    pluginSetting = nullptr;
    loaded.setAutoDelete(true);
}

PluginManager::~PluginManager()
{
    delete pluginSetting;
}

void PluginManager::loadPluginList()
{
    pluginsMetaData = KPluginLoader::findPlugins(QStringLiteral("komport"));

    if (pluginsMetaData.isEmpty())
    {
#if defined(Q_OS_WIN)
    pluginsMetaData = KPluginLoader::findPlugins(QCoreApplication::applicationDirPath()+"/plugins");
#elif defined(Q_OS_LINUX)
    QStringList paths = QCoreApplication::libraryPaths();

    if (paths.isEmpty())
        paths << QLatin1String("/home/work1/QProject/projects/komport/build/exec/usr/lib/qt/plugins");
    QCoreApplication::addLibraryPath(paths.first().remove(QLatin1String("qt5/")));
    pluginsMetaData = KPluginLoader::findPlugins(QStringLiteral("/home/work1/QProject/projects/komport/build/exec/usr/lib/qt/plugins/komport"));
#endif
    }

    for (const KPluginMetaData &module : pluginsMetaData)
    {
        KPluginInfo pi(module);
        pi.setConfig(KSharedConfig::openConfig()->group(pi.pluginName()));
        pi.load();

        plugins << pi;
    }

    pluginSetting = new PluginSetting(gui);
    pluginSetting->loadManager(this);
    gui->addPrefPage(pluginSetting);
    loadPlugins();
}

void PluginManager::loadPlugins()
{
    int idx = 0;
    for (auto i = plugins.begin(); i != plugins.end(); i++)
    {
        KPluginInfo& pi = *i;
        if (loaded.contains(idx) && !pi.isPluginEnabled())
        {
            // unload it
            unload(pi, idx);
            pi.save();
        }
        else if (!loaded.contains(idx) && pi.isPluginEnabled())
        {
            // load it
            load(pi, idx);
            pi.save();
        }
        idx++;
    }
}

void PluginManager::load(const KPluginInfo& pi, int idx)
{
    Q_UNUSED(pi)
    KPluginLoader loader(pluginsMetaData.at(idx).fileName());
    KPluginFactory *factory = loader.factory();
    if (!factory)
        return;

    Plugin* plugin = factory->create<Komport::Plugin>();
    if (!plugin)
    {
        qDebug() << QStringLiteral("Creating instance of plugin %1 failed !")
                                     .arg(pluginsMetaData.at(idx).fileName()) << Qt::endl;
        return;
    }

    if (!plugin->versionCheck(QString::fromLatin1(Komport::VERSION_STRING)))
    {
        qDebug() << QStringLiteral("Plugin %1 version does not match Komport version, unloading it.")
                                     .arg(pluginsMetaData.at(idx).fileName()) << Qt::endl;

        delete plugin;
    }
    else
    {
        plugin->setGUI(gui);
        plugin->setKSerialport(serialport);
        plugin->setSocket(socket);
        plugin->load();
        gui->mergePluginGui(plugin);
        plugin->loaded = true;
        loaded.insert(idx, plugin, true);
    }
}

void PluginManager::unload(const KPluginInfo& pi, int idx)
{
    Q_UNUSED(pi)

    Plugin* p = loaded.find(idx);
    if (!p)
        return;

    // first shut it down properly
    Komport::WaitJob* wjob = new WaitJob(2000);
    try
    {
        p->shutdown(wjob);
        if (wjob->needToWait())
            Komport::WaitJob::execute(wjob);
        else
            delete wjob;
    }
    catch (Error& err)
    {
        qDebug() << "Error when unloading plugin: " << err.toString() << Qt::endl;
    }

    gui->removePluginGui(p);
    p->unload();
    p->loaded = false;
    loaded.erase(idx);
}

void PluginManager::unloadAll()
{
    // first properly shutdown all plugins
    Komport::WaitJob* wjob = new WaitJob(2000);
    try
    {
        Komport::PtrMap<int, Plugin>::iterator i = loaded.begin();
        while (i != loaded.end())
        {
            Plugin* p = i->second;
            p->shutdown(wjob);
            i++;
        }
        if (wjob->needToWait())
            Komport::WaitJob::execute(wjob);
        else
            delete wjob;
    }
    catch (Error& err)
    {
        qDebug() << "Error when unloading all plugins: " << err.toString() << Qt::endl;
    }

    // then unload them
    Komport::PtrMap<int, Plugin>::iterator i = loaded.begin();
    while (i != loaded.end())
    {
        Plugin* p = i->second;
        gui->removePluginGui(p);
        p->unload();
        p->loaded = false;
        i++;
    }
    loaded.clear();
}

void PluginManager::updateGuiPlugins()
{
    Komport::PtrMap<int, Plugin>::iterator i = loaded.begin();
    while (i != loaded.end())
    {
        Plugin* p = i->second;
        p->guiUpdate();
        i++;
    }
}

}
