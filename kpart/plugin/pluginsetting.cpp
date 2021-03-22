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

#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>

#include <KLocalizedString>
#include <KPluginSelector>

#include <util/log.h>
#include "pluginsetting.h"
#include "settings.h"

namespace Komport
{

PluginSetting::PluginSetting(QWidget *parent) : PrefPageInterface(Settings::self(), i18n("Plugins"), QStringLiteral("plugins"), parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setMargin(0);
    pmw = new KPluginSelector(this);
    connect(pmw, &KPluginSelector::changed, this, &PluginSetting::updateButtons);
    connect(pmw, &KPluginSelector::configCommitted, this, &PluginSetting::updateSettings);
    layout->addWidget(pmw);
}

PluginSetting::~PluginSetting(){}

void PluginSetting::loadManager(PluginManager* pman)
{
    pluginManager = pman;
    pmw->addPlugins(pluginManager->pluginInfoList(), KPluginSelector::IgnoreConfigFile, i18n("Plugins"));
    pmw->updatePluginsState();
    pluginManager->loadPlugins();
}

void PluginSetting::loadDefaults()
{
}

void PluginSetting::loadSettings()
{
    pmw->updatePluginsState();
    pluginManager->loadPlugins();
}

void PluginSetting::updateSettings()
{
    pmw->updatePluginsState();
    pluginManager->loadPlugins();
}

}
