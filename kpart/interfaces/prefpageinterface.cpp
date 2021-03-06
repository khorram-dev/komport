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

#include "prefpageinterface.h"

namespace Komport
{
PrefPageInterface::PrefPageInterface(KConfigSkeleton* cfg, const QString& name, const QString& icon, QWidget* parent)
    : QWidget(parent), cfg(cfg), name(name), icon(icon)
{}


PrefPageInterface::~PrefPageInterface()
{}

void PrefPageInterface::loadSettings()
{}

void PrefPageInterface::loadDefaults()
{}

void PrefPageInterface::updateSettings()
{}

//void PrefPageInterface::hasChanged()
//{}

}

