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

#include "udp.h"

namespace Komport {
UDP::UDP(QObject *parent) : QUdpSocket(parent)
{
    socket = new QUdpSocket();
}

bool UDP::bindPort(QString addr, quint16 port)
{
    socket->abort();
    bool isBinded = socket->bind(QHostAddress(addr), port);
    if (isBinded)
    {
        connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    }
    return isBinded;
}

void UDP::sendMessage(QString sender, quint16 senderPort, QString string)
{
    QByteArray Data;
    Data.append(string.toLocal8Bit());

    // Sends the datagram datagram
    // to the host address and at port.
    socket->writeDatagram(Data, QHostAddress(sender), senderPort);
}

void UDP::readyRead()
{
    // when data comes in
    QByteArray buffer;
    buffer.resize(static_cast<int>(socket->pendingDatagramSize()));

    QHostAddress sender;
    quint16 senderPort;

    // Receives a datagram no larger than maxSize bytes and stores it in data.
    // The sender's host address and port is stored in *address and *port
    // (unless the pointers are 0).

    socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

    emit newMessage(sender.toString(), buffer);
}

void UDP::unbindPort()
{
    disconnect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    socket->close();
}






// UDP
void UDP::onUdpButtonClicked()
{
//    if (setupConnection(UDPSERVER))
//    {
//        connect(udp, &UDP::newMessage, this, &socketGUI::onUdpAppendMessage);
//        statusNetworkText = "onUdpStopButtonClicked";
//        Q_EMIT statusNetwork(this, "onUdpStopButtonClicked");
//    }
//    else
//    {
//        statusNetworkText = "onUdpFailed";
//        Q_EMIT statusNetwork(this, "onUdpFailed");
//    }
}

void UDP::onUdpStopButtonClicked()
{
//    disconnect(udp, &UDP::newMessage, this, &socketGUI::onUdpAppendMessage);
//    udp->unbindPort();
//    statusNetworkText = "onUdpButtonClicked";
//    Q_EMIT statusNetwork(this, "onUdpButtonClicked");
}

void UDP::onUdpAppendMessage(const QString &from, const QString &message)
{
    if (from.isEmpty() || message.isEmpty())
        return;

//    QString BufferIn = message;
//    if (BTimestamp)
//        BufferIn = QDateTime::currentDateTime().toString("hh:mm:ss") + " "+ BufferIn;
//    if (BNewLineAfter)
//        BufferIn = BufferIn+"\n";

////    pteLog->moveCursor(QTextCursor::End);
////    pteLog->insertPlainText(from+":> "+BufferIn);
//    m_doc->insertText(m_view->cursorPosition(), BufferIn);
//    Q_EMIT readBuffer(from, message);
}

void UDP::onUdpSendMessage()
{
//    QString text = leSend->text();
//    if (text.isEmpty())
//    {
//        return;
//    }
//    udp->sendMessage(ip, port, text);
//    leSend->clear();
}

}
