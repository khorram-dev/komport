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

#include "tcpserver.h"

namespace Komport {
TCPServer::TCPServer(const QString &ip, quint16 port, QObject *parent) :  QObject(parent), ip(ip), port(port)
{
    tcpServer = new QTcpServer(this);
    tcpServer->setMaxPendingConnections(15);
}

TCPServer::~TCPServer(){}

void TCPServer::listen()
{
    if (!tcpServer->isListening()) {
        isSuccess = tcpServer->listen(QHostAddress(ip), port);
        connect(tcpServer, &QTcpServer::newConnection, this, &TCPServer::onConnected);
        statusNetworkText = "onTcpServerStopButtonClicked";
        Q_EMIT serverConnected(1);
    }
}

bool TCPServer::toListen(QString addr, quint16 port)
{
    if(!tcpServer->isListening()) {
        isSuccess = tcpServer->listen(QHostAddress(addr), port);
        if (isSuccess)
            connect(tcpServer, &QTcpServer::newConnection, this, &TCPServer::onConnected);
    }
    return isSuccess;
}

void TCPServer::onConnected()
{
    QTcpSocket *tcpSocket = tcpServer->nextPendingConnection();
    connectedClients.push_back(tcpSocket);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &TCPServer::messageReady);
    connect(tcpSocket, &QTcpSocket::stateChanged, this, &TCPServer::onDisconnected);
//    Q_EMIT serverConnected(tcpSocket->peerAddress().toString(), tcpSocket->peerPort());
    statusNetworkText = "onTcpServerDisconnectButtonClicked";
    Q_EMIT serverConnected(0);
}

void TCPServer::sendMessage(const QString& message)
{
//    QTcpSocket *sender = static_cast<QTcpSocket*>(QObject::sender());
    for (QTcpSocket *client : qAsConst(connectedClients)) {
        if (client->state() == QTcpSocket::ConnectedState) {
            client->write(sendString(message).toLocal8Bit());
            client->flush();
        }
        client->waitForBytesWritten(timedelay);
    }
}

void TCPServer::messageReady()
{
    QTcpSocket *sender = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray data = sender->readAll();
//    data = sender->protocolTag().toUtf8() + ":> " + data;
    Q_EMIT newMessage(data);

    for (QTcpSocket *client : qAsConst(connectedClients)) {
        if (client != sender) {
            client->write(data);
            client->flush();
        }
    }
}

void TCPServer::onDisconnected(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState) {
        QTcpSocket *client = static_cast<QTcpSocket*>(QObject::sender());
        connectedClients.removeOne(client);
    }
    if(connectedClients.count() <= 0) {
        statusNetworkText = "onTcpServerStopButtonClicked";
        Q_EMIT serverConnected(0);
//        Q_EMIT serverDisconnected();
    }
}

void TCPServer::stopConnection()
{
    for (QTcpSocket *client : qAsConst(connectedClients)) {
        client->disconnectFromHost();
        connectedClients.removeOne(client);
    }
    statusNetworkText = "onTcpServerNewConnection";
    Q_EMIT serverConnected(0);
}

void TCPServer::stopListening()
{
    if (isSuccess)
    {
        disconnect(tcpServer,nullptr,nullptr,nullptr);
        tcpServer->close();
        statusNetworkText = "onTcpServerButtonClicked";
        Q_EMIT serverConnected(0);
    }
    isSuccess = false;
}

bool TCPServer::status()
{
    return tcpServer->isListening();
}

QString TCPServer::sendString(QString text)
{
    QString message;
    if (lineMode == LineMode::HEX)
    {
        text.remove(QRegExp("\\s+(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)")); // spaces except that in quotes
        if ((text.startsWith("0x")) || (text.startsWith("0X"))) {
            text = text.mid(2);
        }

        bool ascii = false;
        for (int i = 0; i < text.length();) {
            QString nextByte = text.mid(i, ascii ? 1 : 2);
            i += ascii ? 1 : 2;
            if (nextByte.at(0) == "\"") {
                if (!ascii)
                    ascii = true;
                else {
                    ascii = false;
                    continue;
                }
            }
            if (ascii)
                continue;
            bool ok = true;
            nextByte.toUInt(&ok, 16);
            if (!ok) {
                qDebug() << tr("Invalid hex characters")
                         << tr("The input string contains invalid hex characters: 0x%1").arg(nextByte)
                         << Qt::endl;
            }
        }

        if (ascii) {
            qDebug() << tr("Invalid format")
                     << tr("No closing quote")
                     << Qt::endl;
        }

        for (int i = 0; i < text.length();) {
            QString nextByte = text.mid(i, ascii ? 1 : 2);
            i += ascii ? 1 : 2;
            if (nextByte.at(0) == "\"") {
                if (!ascii) {
                    ascii = true;
                    nextByte = nextByte.right(1);
                } else {
                    ascii = false;
                    continue;
                }
            }
            unsigned int byte;
            if (ascii)
                byte = nextByte.at(0).unicode() & 0xFF;
            else
                byte = nextByte.toUInt(nullptr, 16);

            message = static_cast<QString>(byte & 0xff);
        }

    } else {
        message = text;
        switch (lineMode) {
        case LineMode::LF:
            message.append('\n');
            break;
        case LineMode::CR:
            message.append('\r');
            break;
        case LineMode::CRLF:
            message.append('\r');
            message.append('\n');
            break;
        default:
            break;
        }
    }
    return message;
}

void TCPServer::sendFile(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Some error occur. Could not open the file.";
        return;
      }

    QFileInfo fileInfo(file);

    qInfo() << "Filename:" << path << "; size:" << fileInfo.size();
    for (QTcpSocket *client : qAsConst(connectedClients)) {

    QDataStream out(client);

    out << path;
    out << QString::number(fileInfo.size());

    while (!file.atEnd()) {
        QByteArray rawFile = file.read(5000);
        out << rawFile;
        int mSize = rawFile.size();
        qInfo() << rawFile;
      }
    }
}

}
