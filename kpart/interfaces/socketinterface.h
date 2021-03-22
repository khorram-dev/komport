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

#ifndef SOCKETINTERFACE_H
#define SOCKETINTERFACE_H

#include <QList>
#include <interfaces/activity.h>
#include <KMultiTabBar>
#include <libkomport_export.h>

namespace Komport {
/**
    Interface for the Socket class
*/
class LIBKOMPORT_EXPORT SocketInterface : public Activity
{
    Q_OBJECT
public:
    SocketInterface(const QString& name, const QString& icon, QWidget* parent);
    ~SocketInterface() override;

    /// Add a view listener.
    void addViewListener(ViewListener* vl);

    /// Remove a view listener
    void removeViewListener(ViewListener* vl);

    /// Update all actions
    virtual void updateActions() = 0;

protected:
    /**
    * Notifies all view listeners of the change in the current downloading QObject
    * @param tc Pointer to current QObject
    */
    void notifyViewListeners(QObject* tc);

private:
    QList<ViewListener*> listeners;

//Q_SIGNALS:
//    void readBuffer(const QString&, const QString&);


public:
//    SocketInterface();
//    virtual ~SocketInterface();
    virtual void sendText(QString text, int time) = 0;

};
}

#endif // SOCKETINTERFACE_H

