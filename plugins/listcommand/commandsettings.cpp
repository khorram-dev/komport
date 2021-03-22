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

#include <KSharedConfig>
#include <KLocalizedString>
#include "commandpluginsettings.h"
#include "commandsettings.h"

namespace Komport
{
commandSettings::commandSettings(QWidget* parent) : PrefPageInterface(commandPluginSettings::self(), i18n("List Command"), QStringLiteral("view-sidetree"), parent)
{
    setupUi(this);
    kcfg_saveDir->setMode(KFile::Directory | KFile::ExistingOnly | KFile::LocalOnly);
}

commandSettings::~commandSettings(){}


void commandSettings::loadSettings()
{
    kcfg_saveDir->setProperty("kcfg_property", QStringLiteral("text"));
    if (commandPluginSettings::saveDir().isEmpty())
        kcfg_saveDir->setText(qApp->applicationDirPath()+"/listfiles");
    else
        kcfg_saveDir->setText(commandPluginSettings::saveDir());
//    kcfg_lineedit->setText(commandPluginSettings::lineedit());
}

void commandSettings::loadDefaults()
{
    if(!QDir(qApp->applicationDirPath()+"/listfiles").exists())
        QDir(qApp->applicationDirPath()).mkdir("listfiles");
    commandPluginSettings::setSaveDir(qApp->applicationDirPath()+"/listfiles");
    loadSettings();
}

}
