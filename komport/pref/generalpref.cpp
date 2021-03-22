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

#include "generalpref.h"
#include "settings.h"
#include <QColor>

namespace Komport
{
GeneralPref::GeneralPref(QWidget* parent) : PrefPageInterface(Settings::self(), i18n("Application"), QStringLiteral("preferences-other"), parent)
{
    setupUi(this);
    kcfg_serialportDir->setMode(KFile::Directory | KFile::ExistingOnly | KFile::LocalOnly);
    kcfg_socketDir->setMode(KFile::Directory | KFile::ExistingOnly | KFile::LocalOnly);
}

GeneralPref::~GeneralPref(){}

void GeneralPref::loadSettings()
{
    kcfg_serialportDir->setProperty("kcfg_property", QStringLiteral("text"));
    if (Settings::serialportDir().isEmpty())
        kcfg_serialportDir->setText(qApp->applicationDirPath()+"/serialport");
    else
        kcfg_serialportDir->setText(Settings::serialportDir());

    kcfg_socketDir->setProperty("kcfg_property", QStringLiteral("text"));
    if (Settings::socketDir().isEmpty())
        kcfg_socketDir->setText(qApp->applicationDirPath()+"/socket");
    else
        kcfg_socketDir->setText(Settings::socketDir());

//    kcfg_iconColor->setDefaultColor(QColor("#55557f"));
//    kcfg_iconColor->setColor(QColor("#55557f"));
}

void GeneralPref::loadDefaults()
{
//    kcfg_iconColor->setDefaultColor(QColor("#55557f"));
//    kcfg_iconColor->setColor(QColor("#55557f"));
    if(!QDir(qApp->applicationDirPath()+"/serialport").exists()) {
        QDir(qApp->applicationDirPath()).mkdir("serialport");
    }
        Settings::setSerialportDir(qApp->applicationDirPath()+"/serialport");

    if(!QDir(qApp->applicationDirPath()+"/socket").exists())
        QDir(qApp->applicationDirPath()).mkdir("socket");
    Settings::setSocketDir(qApp->applicationDirPath()+"/socket");

    loadSettings();
}

}
