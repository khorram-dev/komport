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

#ifndef UDP_H
#define UDP_H

#include <QUdpSocket>
#include <libkomport_export.h>

namespace Komport {
class LIBKOMPORT_EXPORT UDP : public QUdpSocket
{
    Q_OBJECT

public:
    explicit UDP(QObject *parent = nullptr);
    bool bindPort(QString addr, quint16 port);
    void unbindPort();

Q_SIGNALS:
    void newMessage(const QString &from, const QString &message);

public Q_SLOTS:
    void readyRead();
    void sendMessage(QString sender, quint16 senderPort, QString string);

    // UDP
    void onUdpButtonClicked();
    void onUdpStopButtonClicked();
    void onUdpSendMessage();
    void onUdpAppendMessage(const QString &from, const QString &message);

private:
    QUdpSocket *socket;
};
}

#endif // UDP_H
