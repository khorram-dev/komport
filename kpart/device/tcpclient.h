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

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QUrl>
#include <QFile>
#include <QDataStream>
#include <libkomport_export.h>

namespace Komport {
class LIBKOMPORT_EXPORT TCPClient : public QObject
{
    Q_OBJECT
public:
    explicit TCPClient(const QString &ip, quint16 port, QObject *parent = nullptr);

    void connectTo(QString addr, quint16 port);
    void sendMessage(const QString &message);


    void sendFile(QString path);
    bool status();


    QString statusNetworkText;
    int lineMode;
    int timedelay=1000;

public Q_SLOTS:
    void openConnect();
    void disconnectCurrentConnection();
    void closeClient();
    void abortConnection();
    void onDisconnected();
    void onConnected();
private:
    QTcpSocket *tcpSocket;
    const QString &ip;
    quint16 port;
    QString sendString(QString text);
    enum LineMode { NONE = 0, LF, CR, CRLF, HEX };

private Q_SLOTS:


    void messageReady();
    void onStateChanged(QAbstractSocket::SocketState state);

Q_SIGNALS:
    void newMessage(QByteArray message);
    void clientConnected(int status);
//    void clientConnected(const QString &from, quint16 port);
    void clientDisconnected();
    void connectionFailed();

};
}

#endif // TCPCLIENT_H
