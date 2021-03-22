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

#include "tcpclient.h"

namespace Komport {
TCPClient::TCPClient(const QString &ip, quint16 port, QObject *parent) : QObject(parent), ip(ip), port(port)
{
    tcpSocket = new QTcpSocket(this);
}

void TCPClient::openConnect()
{
    tcpSocket->connectToHost(QHostAddress(ip), port);
    connect(tcpSocket, &QTcpSocket::connected, this, &TCPClient::onConnected);
    connect(tcpSocket, &QTcpSocket::stateChanged, this, &TCPClient::onStateChanged);
}

void TCPClient::connectTo(QString addr, quint16 port)
{
    tcpSocket->connectToHost(QHostAddress(addr), port);
    connect(tcpSocket, &QTcpSocket::connected, this, &TCPClient::onConnected);
    connect(tcpSocket, &QTcpSocket::stateChanged, this, &TCPClient::onStateChanged);
}

void TCPClient::onConnected()
{
    disconnect(tcpSocket, &QTcpSocket::stateChanged, this, &TCPClient::onStateChanged);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &TCPClient::onDisconnected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &TCPClient::messageReady);
    statusNetworkText = "onTcpClientButtonClicked";
    Q_EMIT clientConnected(1);
//    Q_EMIT clientConnected(tcpSocket->peerAddress().toString(), tcpSocket->peerPort());
}

void TCPClient::onStateChanged(QAbstractSocket::SocketState state)
{
    disconnect(tcpSocket, &QTcpSocket::stateChanged, this, &TCPClient::onStateChanged);
    switch (state)
    {
    case QAbstractSocket::UnconnectedState:
        statusNetworkText = "onTcpClientTimeOut";
        Q_EMIT clientConnected(0);
//        Q_EMIT connectionFailed();
        //qDebug()<<"connecting timeout";
        break;
    case QAbstractSocket::HostLookupState:
        //qDebug()<<"HostLookupState";
        break;
    case QAbstractSocket::ConnectingState:
        //qDebug()<<"ConnectingState";
        break;
    case QAbstractSocket::ConnectedState:
        //qDebug()<<"ConnectedState";
        break;
    case QAbstractSocket::BoundState:
        //qDebug()<<"BoundState";
        break;
    case QAbstractSocket::ListeningState:
        //qDebug()<<"ListeningState";
        break;
    case QAbstractSocket::ClosingState:
        //qDebug()<<"ClosingState";
        break;
    }
}

void TCPClient::onDisconnected()
{
    disconnect(tcpSocket, SIGNAL(disconnected()));
    disconnect(tcpSocket, SIGNAL(readyRead()));
    tcpSocket->close();
//    Q_EMIT clientDisconnected();
    statusNetworkText = "onTcpClientDisconnected";
    Q_EMIT clientConnected(0);
}

void TCPClient::closeClient()
{
    tcpSocket->close();
}

void TCPClient::abortConnection()
{
    tcpSocket->abort();
}

void TCPClient::messageReady()
{
    QByteArray data = tcpSocket->readAll();
    Q_EMIT newMessage(data);
}

void TCPClient::sendMessage(const QString& message)
{
    if (tcpSocket->state() == QTcpSocket::ConnectedState)
    {
        tcpSocket->write(sendString(message).toLocal8Bit());
        tcpSocket->waitForBytesWritten(timedelay);
        tcpSocket->flush();
    }
}

void TCPClient::sendFile(QString path)
{
    QFile file(path);
    if (!file.open(QFile::ReadOnly))
    {
        qDebug() << tr("Could not open the file for reading");
        return;
    }
    QByteArray mydata = file.readAll();
    if (tcpSocket->state() == QTcpSocket::ConnectedState)
    {
        tcpSocket->write(mydata, mydata.size());
        tcpSocket->waitForBytesWritten(timedelay);
        tcpSocket->flush();
    }
    file.close();
}

void TCPClient::disconnectCurrentConnection()
{
    tcpSocket->disconnectFromHost();
}

bool TCPClient::status()
{
    return tcpSocket->isOpen();
}

QString TCPClient::sendString(QString text)
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


}
