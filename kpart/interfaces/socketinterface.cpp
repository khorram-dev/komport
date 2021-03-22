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

#include "socketinterface.h"

namespace Komport
{
SocketInterface::SocketInterface(const QString& name, const QString &icon, QWidget* parent)
    : Activity(name, icon, 0, parent){}

SocketInterface::~SocketInterface(){}

void SocketInterface::notifyViewListeners(QObject* tc)
{
    for (ViewListener* vl : qAsConst(listeners))
        vl->currentKomportChanged(tc);
}

void SocketInterface::addViewListener(ViewListener* vl)
{
    listeners.append(vl);
}

void SocketInterface::removeViewListener(ViewListener* vl)
{
    listeners.removeAll(vl);
}

}
