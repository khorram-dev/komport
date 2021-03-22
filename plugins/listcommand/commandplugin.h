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

#ifndef KTCOMMANDPLUGIN_H
#define KTCOMMANDPLUGIN_H

#include <interfaces/plugin.h>
#include <interfaces/activity.h>

namespace Komport
{
class guiCommand;
class commandSettings;

class CommandPlugin : public Plugin
{
    Q_OBJECT
public:
    CommandPlugin(QObject* parent, const QVariantList& args);
    ~CommandPlugin() override;

    void load() override;
    void unload() override;
    bool versionCheck(const QString& version) const override;
//    void guiUpdate() override;
    void sendTerminal(QString text, int time);

private:
    void setupTab();

private:
    guiCommand* lv;
    commandSettings* pref;
    QWidget *toolview;
};

}

#endif
