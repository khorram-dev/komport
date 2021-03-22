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

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QFile>
#include <QFileInfo>
#include <QUrl>
#include <QDataStream>
#include <libkomport_export.h>

namespace Komport {
class LIBKOMPORT_EXPORT TCPServer : public QObject
{
    Q_OBJECT

public:
    explicit TCPServer(const QString &ip, quint16 port, QObject *parent = nullptr);
    ~TCPServer();

    bool toListen(QString addr, quint16 port);
    void sendFile(QString path);


    bool status();
    QTcpServer *tcpServer;

    int bytesToWrite;

    QString statusNetworkText;

    QString sendString(QString text);
    int lineMode;
    int timedelay=1000;

public Q_SLOTS:
    void listen();
    void sendMessage(const QString &message);
    void stopConnection();
    void stopListening();


//private Q_SLOTS:
    void onConnected();
    void onDisconnected(QAbstractSocket::SocketState socketState);
    void messageReady();

private:
//    QTcpSocket *tcpSocket;
    const QString &ip;
    quint16 port;

    QByteArray array;
    QHostAddress clientAddr;
    quint16 clientPort;

    const QByteArray welcomeMessage = "<----+ [ Welcome to server ] +---->\n";
    QVector<QTcpSocket*> connectedClients;
    bool isSuccess=false;




    enum LineMode { NONE = 0, LF, CR, CRLF, HEX };

Q_SIGNALS:
    void newMessage(QByteArray message);
//    void serverConnected(const QString &from, quint16 port);
    void serverConnected(int status);
    void serverDisconnected();
};
}
#endif // TCPSERVER_H
