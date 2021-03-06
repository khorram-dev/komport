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

#ifndef KTPLUGINACTIVITY_H
#define KTPLUGINACTIVITY_H

#include <QWidget>
#include <interfaces/activity.h>

class KPluginSelector;

namespace Komport
{
class PluginManager;

/**
* @author Masoud Khorram
*
* Pref page which allows to load and unload plugins.
*/
class PluginActivity : public Activity
{
    Q_OBJECT

public:
    PluginActivity(PluginManager* pman);
    ~PluginActivity() override;

    void updatePluginList();
    void update();

private Q_SLOTS:
    void changed();

private:
    PluginManager* pman;
    KPluginSelector* pmw;
};

}

#endif
