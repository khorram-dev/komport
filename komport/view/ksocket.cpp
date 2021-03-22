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

//> The "New" button could be named "open port". TCP and RS232 open dialogs
//> could be split into two "Open Serial" and "Open TCP" because the settings
//> have nothing in common.

//> * Allow to set a profile as startup default.
//> * Provide a hex/printable hybrid mode like hexeditors or gtkterm do.
//> * Provide a normal Makefile or a readme on how to generate one.


#include "ksocket.h"
#include <QSettings>
#include <QMenu>
#include <KLocalizedString>
#include <settings.h>
#include <QFile>
#include <QFileDialog>
#include <QProgressDialog>
#include "statusbar.h"
#include "util/ksvg.h"
#include "pref/generalpref.h"
#include "gui/klistwidget.h"
#include "editor.h"

namespace Komport {

KSocket::KSocket(QWidget *parent) : SocketInterface(i18n("Socket"), ":/images/svg/usbport.svg", parent)
{
    setXMLGUIFile(QStringLiteral("socketui.rc"));
    setContentsMargins(0, 0, 0, 0);
    tabWidget = new QTabWidget(this);
    tabWidget->setContentsMargins(0,0,0,0);
    QVBoxLayout* mlayout = new QVBoxLayout(this);
    mlayout->setContentsMargins(0,0,0,0);
    mlayout->setSpacing(0);
    mlayout->setMargin(0);
    mlayout->addWidget(tabWidget);
    listWidget = new KListWidget(this);
    reloadItems();
    listWidget->setContentsMargins(0,0,0,0);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setAcceptDrops(true);
    tabWidget->addTab(listWidget, Ksvg::seticonSVG(":/images/svg/komport.svg", Settings::iconColor()), i18n("List View"));
    tabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, nullptr);
    doDevice = new newDevice(parent);
    connect(doDevice, &newDevice::openFile, this, &KSocket::openFile);
    connect(doDevice, &newDevice::deviceReload, listWidget, &KListWidget::readItems);
    connect(tabWidget, &QTabWidget::currentChanged, this, &KSocket::showMenu);
    connect(tabWidget, &QTabWidget::currentChanged, this, &KSocket::currectTabWidget);
    connect(listWidget, &KListWidget::runFile, this, &KSocket::openFile);
    connect(listWidget, &KListWidget::editItem, this, &KSocket::editItem);
    sockets.insert(0, {{"NULL", new QObject}});
    editors.append(new KEditor);
}

KSocket::~KSocket(){}

void KSocket::reloadItems()
{
    if(Settings::socketDir().isEmpty())
        listWidget->setPath(qApp->applicationDirPath()+"/socket");
    else
        listWidget->setPath(Settings::socketDir());
    listWidget->readItems();
}

void KSocket::setupActions(KActionCollection* ac)
{
//    Action newDevice
    QAction *newProfile = new QAction(Ksvg::seticonSVG(":/images/svg/document-new.svg", Settings::iconColor()), i18n("New"), this);
	newProfile->setToolTip(i18n("New Profile"));
    connect(newProfile, &QAction::triggered, this, &KSocket::newFile);
    ac->addAction(QStringLiteral("ac_new"), newProfile);
    ac->setDefaultShortcut(newProfile, QKeySequence(Qt::CTRL + Qt::Key_N));

    QAction *openProfile = new QAction(Ksvg::seticonSVG(":/images/svg/document-open.svg", Settings::iconColor()), i18n("Open"), this);
	openProfile->setToolTip(i18n("Open Profile"));
    connect(openProfile, &QAction::triggered, doDevice, &newDevice::openProfile);
    ac->addAction(QStringLiteral("ac_open"), openProfile);
    ac->setDefaultShortcut(openProfile, QKeySequence(Qt::CTRL + Qt::Key_O));

//    Action socketGUI
    pbOpen = new QAction(Ksvg::seticonSVG(":/images/svg/plug_connect.svg", Settings::iconColor()), i18n("Connect"), this);
	pbOpen->setToolTip(i18n("Connect in the current tab"));
    pbOpen->setEnabled(false);
    ac->addAction(QStringLiteral("ac_connection"), pbOpen);
    ac->setDefaultShortcut(pbOpen, QKeySequence(Qt::CTRL + Qt::Key_Enter));

    cbNewLineAfter = new QAction(Ksvg::seticonSVG(":/images/svg/newline.svg", Settings::iconColor()), i18n("New Line"), this);
	cbNewLineAfter->setToolTip(i18n("New line each time"));
    cbNewLineAfter->setEnabled(false);
    cbNewLineAfter->setCheckable(true);
    cbNewLineAfter->setChecked(true);
    ac->addAction(QStringLiteral("ac_newlineafter"), cbNewLineAfter);
    ac->setDefaultShortcut(cbNewLineAfter, QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N));

    cbTimestamp = new QAction(Ksvg::seticonSVG(":/images/svg/time-left.svg", Settings::iconColor()), i18n("Timestamp"), this);
	cbTimestamp->setToolTip(i18n("Append Time & Date in terminal"));
    cbTimestamp->setEnabled(false);
    cbTimestamp->setCheckable(true);
    cbTimestamp->setChecked(false);
    ac->addAction(QStringLiteral("ac_timestamp"), cbTimestamp);
    ac->setDefaultShortcut(cbTimestamp, QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_T));

    pbSendFile = new QAction(Ksvg::seticonSVG(":/images/svg/document-send.svg", Settings::iconColor()), i18n("Send File"), this);
	pbSendFile->setToolTip(i18n("send file"));
    pbSendFile->setEnabled(false);
    ac->addAction(QStringLiteral("ac_sendfile"), pbSendFile);
    ac->setDefaultShortcut(pbSendFile, QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F));

    pbClose = new QAction(Ksvg::seticonSVG(":/images/svg/window-close.svg", Settings::iconColor()), i18n("Close"), this);
	pbClose->setToolTip(i18n("close Comport in the current tab"));
    pbClose->setEnabled(false);
    ac->addAction(QStringLiteral("ac_close"), pbClose);
    ac->setDefaultShortcut(pbClose, QKeySequence(Qt::CTRL + Qt::Key_W));
}

void KSocket::updateActions(){}

void KSocket::newFile()
{
    doDevice->newFile("socket");
}

void KSocket::showMenu()
{
    QMenu* view_menu = part()->menu(QStringLiteral("ViewMenu"));
    if (!view_menu)
        return;
    part()->unplugActionList(QStringLiteral("view_columns_list"));
//    part()->plugActionList(QStringLiteral("view_columns_list"), column_action_list);
}

void KSocket::editItem(const QString& path)
{
    doDevice->socketName->setText(path);
    doDevice->editProfile("socket", listWidget->path);
}

void KSocket::openFile(const QString& fileName)
{
    QSettings *settings = new QSettings(fileName, QSettings::IniFormat);
    settings->beginGroup("Socket");
    QString fName = settings->value("FileName").toString();
    socketType = settings->value("Type").toString();
    ip = settings->value("IP").toString();
    QString portString = settings->value("PORT").toString();
    QTextStream ts(&portString);
    ts >> port;

    editor = new KEditor;
    connect(editor, &KEditor::linemodeChanged, this, &KSocket::setLineMode);
    connect(editor, &KEditor::changeTimer, this, &KSocket::changeTimer);
    editors.append(editor);
    
    if(socketType == "TCP Server") {
        tabWidget->addTab(editor, Ksvg::seticonSVG(":/images/svg/tcpserver.svg", Settings::iconColor()), fName);
        TCPServer* tcpServer = new TCPServer(ip, port);
        sockets.insert(COUNTER, {{socketType ,tcpServer}});
        connect(tcpServer, &TCPServer::newMessage, editor, &KEditor::plainText);
        connect(tcpServer, &TCPServer::serverConnected, this, &KSocket::currectTabWidget);
        connect(editor, &KEditor::sendText, tcpServer, &TCPServer::sendMessage);
        tcpServer->listen();
        tcpServer->lineMode = Settings::lineMode();
    } else if(socketType == "TCP Client") {
        tabWidget->addTab(editor, Ksvg::seticonSVG(":/images/svg/tcpclient.svg", Settings::iconColor()), fName);
        TCPClient* tcpClient = new TCPClient(ip, port);
        sockets.insert(COUNTER, {{socketType ,tcpClient}});
        connect(tcpClient, &TCPClient::newMessage, editor, &KEditor::plainText);
        connect(tcpClient, &TCPClient::clientConnected, this, &KSocket::currectTabWidget);
        connect(editor, &KEditor::sendText, tcpClient, &TCPClient::sendMessage);
        tcpClient->openConnect();
        tcpClient->lineMode = Settings::lineMode();
    } else if(socketType == "UDP") {
        UDP* udp = new UDP;
        udp->bindPort(ip, port);
//        udps.append(udp);
//        sockets.insert(socketType ,udp);
        sockets.insert(COUNTER, {{socketType ,udp}});
//        connect(udp, &UDP::newMessage, editor, &KEditor::plainText);
//        connect(editor, &KEditor::sendText, udp, &UDP::sendMessage);
//        onUdpButtonClicked();
        tabWidget->addTab(editor, Ksvg::seticonSVG(":/images/svg/udp.svg", Settings::iconColor()), fName);
    }

    editor->timeStamp = Settings::timeStamp();
    editor->newLineAfter = Settings::newLine();
    cbTimestamp->setChecked(Settings::timeStamp());
    cbNewLineAfter->setChecked(Settings::newLine());
//    connect(gui, &GUI::settingsChanged, socketGui, &KSocket::applySettings);
//    tabWidget->setCurrentIndex(tabWidget->count()-1);
    tabWidget->widget(tabWidget->count()-1)->setObjectName(QString::number(COUNTER++));
}

void KSocket::setLineMode(int index)
{
    if(socketType == "TCP Server") {
        currentTcpServer()->lineMode = index;
    } else if(socketType == "TCP Client") {
        currentTcpClient()->lineMode = index;
    } else if(socketType == "UDP") {
//        statusNetwork(currentTcpServer()->statusNetworkText);
    }
}

void KSocket::changeTimer(int index)
{
    if(socketType == "TCP Server") {
        currentTcpServer()->timedelay = index;
    } else if(socketType == "TCP Client") {
        currentTcpClient()->timedelay = index;
    } else if(socketType == "UDP") {
//        statusNetwork(currentTcpServer()->statusNetworkText);
    }
}

void KSocket::statusNetwork(const QString &statusName)
{
    disconnect(pbOpen,nullptr,nullptr,nullptr);
    if(tabWidget->currentIndex() > 0) {
        if(socketType == "TCP Server") {
            // TCPServer
            if(statusName == "onTcpServerNewConnection") {
                connect(pbOpen, &QAction::triggered, currentTcpServer(), &TCPServer::stopListening);
                pbOpen->setText(i18n("Stop"));
                pbOpen->setIcon(Ksvg::seticonSVG(":/images/svg/plug_connect.svg", "#00ff7f"));
                currentEditor()->setDisabled(true);
        //            gui->status_bar->showMessage(messageTCPServer + "Client disconnected, listerning to " + currentSocket()->ip + ": " + QString::number(currentSocket()->port), 0);
            } else if(statusName == "onTcpServerButtonClicked") {
                connect(pbOpen, &QAction::triggered, currentTcpServer(), &TCPServer::listen);
                pbOpen->setText(i18n("Start"));
                pbOpen->setIcon(Ksvg::seticonSVG(":/images/svg/plug_disconnect.svg", "#ff0000"));
                currentEditor()->setDisabled(true);
        //        gui->status_bar->showMessage(messageTCPServer + "Stopped", 5000);
            } else if(statusName == "onTcpServerDisconnectButtonClicked") {
                connect(pbOpen, &QAction::triggered, currentTcpServer(), &TCPServer::stopConnection);
                pbOpen->setText(i18n("Disconnect"));
                pbOpen->setIcon(Ksvg::seticonSVG(":/images/svg/plug_connected.svg", "#5500ff"));
                currentEditor()->setEnabled(true);
        //        gui->status_bar->showMessage(messageTCPServer + "Connected with " + currentSocket()->ip + ": " + QString::number(currentSocket()->port), 0);
            } else if(statusName == "onTcpServerStopButtonClicked") {
                connect(pbOpen, &QAction::triggered, currentTcpServer(), &TCPServer::stopListening);
                pbOpen->setText(i18n("Stop"));
                pbOpen->setIcon(Ksvg::seticonSVG(":/images/svg/plug_connect.svg", "#00ff7f"));
                currentEditor()->setDisabled(true);
        //        gui->status_bar->showMessage(messageTCPServer + "Listerning to " + currentSocket()->ip + ": " + QString::number(currentSocket()->port), 0);
            }
        } else if(socketType == "TCP Client") {
            // TCPClient
            if(statusName == "onTcpClientButtonClicked") {
                connect(pbOpen, &QAction::triggered, currentTcpClient(), &TCPClient::onDisconnected);
                pbOpen->setText(i18n("Stop"));
                pbOpen->setIcon(Ksvg::seticonSVG(":/images/svg/plug_connect.svg", "#55aa00"));
                currentEditor()->setEnabled(true);
        //        gui->status_bar->showMessage(messageTCPClient + "Connected with " + currentSocket()->ip + ": " + QString::number(currentSocket()->port), 0);
            } else if(statusName == "onTcpClientDisconnected") {
                connect(pbOpen, &QAction::triggered, currentTcpClient(), &TCPClient::openConnect);
                pbOpen->setText(i18n("Start"));
                pbOpen->setIcon(Ksvg::seticonSVG(":/images/svg/plug_disconnect.svg", "#ff5500"));
                currentEditor()->setDisabled(true);
        //        gui->status_bar->showMessage(messageTCPClient + "Disconnected from server", 5000);
            } else if(statusName == "onTcpClientStopButtonClicked") {
                connect(pbOpen, &QAction::triggered, currentTcpClient(), &TCPClient::openConnect);
                pbOpen->setText(i18n("Start"));
                pbOpen->setIcon(Ksvg::seticonSVG(":/images/svg/plug_disconnect.svg", "#ff0000"));
                currentEditor()->setDisabled(true);
        //        gui->status_bar->showMessage(messageTCPClient + "Stopped", 5000);
            } else if(statusName == "onTcpClientTimeOut") {
                connect(pbOpen, &QAction::triggered, currentTcpClient(), &TCPClient::onDisconnected);
                pbOpen->setText(i18n("Stop"));
                pbOpen->setIcon(Ksvg::seticonSVG(":/images/svg/plug_disconnect.svg", "#ff007f"));
                currentEditor()->setDisabled(true);
        //        gui->status_bar->showMessage(messageTCPClient + "Connection time out", 5000);
            }
        } else if(socketType == "UDP") {
            //UDP
            if(statusName == "onUdpStopButtonClicked") {
        //       connect(pbOpen, &QAction::triggered, this, &KSocket::onUdpStopButtonClicked);
               pbOpen->setText(i18n("Stop"));
               pbOpen->setIcon(Ksvg::seticonSVG(":/images/svg/plug_connect.svg", "#55aa00"));
               currentEditor()->setEnabled(true);
        //       gui->status_bar->showMessage(messageUDPServer + "Connected with " + currentSocket()->ip + ": " + QString::number(currentSocket()->port), 0);
           } else if(statusName == "onUdpButtonClicked") {
        //       connect(pbOpen, &QAction::triggered, this, &KSocket::onUdpButtonClicked);
               pbOpen->setText(i18n("Start"));
               pbOpen->setIcon(Ksvg::seticonSVG(":/images/svg/plug_disconnect.svg", "#ff0000"));
               currentEditor()->setDisabled(true);
        //       gui->status_bar->showMessage(messageUDPServer + "Stopped", 5000);
           } else if(statusName == "onUdpFailed") {
        //        connect(pbOpen, &QAction::triggered, this, &KSocket::onUdpButtonClicked);
                pbOpen->setText(i18n("Ready only Send"));
                pbOpen->setIcon(Ksvg::seticonSVG(":/images/svg/ready.svg", "#ff0000"));
                currentEditor()->setDisabled(true);
        //        gui->status_bar->showMessage(messageUDPServer + "Failed to listen to: " + currentSocket()->ip + ": " + QString::number(currentSocket()->port), 5000);
            }
        }
    }
}

void KSocket::currentUpdate()
{
    bool checkAction = false;
    if(tabWidget->currentIndex() > 0 ) {
//        disconnect(pbSaveToFile, &QAction::triggered, this, &KSocket::saveToFile);
        disconnect(pbSendFile, &QAction::triggered, this, &KSocket::sendFile);
        disconnect(cbTimestamp, &QAction::triggered, this, &KSocket::setMessageState);
        disconnect(cbNewLineAfter, &QAction::triggered, this, &KSocket::setMessageState);
        disconnect(pbClose, &QAction::triggered, this, &KSocket::closeProfile);
        checkAction = true;
        connect(cbTimestamp, &QAction::triggered, this, &KSocket::setMessageState);
        connect(cbNewLineAfter, &QAction::triggered, this, &KSocket::setMessageState);
//        connect(pbSaveToFile, &QAction::triggered, this, &KSocket::saveToFile);
        connect(pbSendFile, &QAction::triggered, this, &KSocket::sendFile);
        connect(pbClose, &QAction::triggered, this, &KSocket::closeProfile);

//        for (socketGUI *sgui : socketGuis) {
//            if(sgui != nullptr) {
//                if(tabWidget->tabText(tabWidget->currentIndex()).isNull())
//                    sgui->currentTabName("null");
//                else
//                    sgui->currentTabName(tabWidget->tabText(tabWidget->currentIndex()).replace("&", ""));
//            }
//        }

        cbTimestamp->setChecked(currentEditor()->timeStamp);
        cbNewLineAfter->setChecked(currentEditor()->newLineAfter);
    }

    pbOpen->setEnabled(checkAction);
    cbTimestamp->setEnabled(checkAction);
    cbNewLineAfter->setEnabled(checkAction);
//    pbSaveToFile->setEnabled(checkAction);
    pbSendFile->setEnabled(checkAction);
    pbClose->setEnabled(checkAction);
}

void KSocket::setMessageState()
{
    currentEditor()->timeStamp = cbTimestamp->isChecked();
    currentEditor()->newLineAfter = cbNewLineAfter->isChecked();
}

void KSocket::sendText(QString text, int time)
{
    if(tabWidget->currentIndex() > 0) {
        if(socketType == "TCP Server") {
            currentTcpServer()->timedelay = time;
            currentTcpServer()->sendMessage(text);
        } else if(socketType == "TCP Client") {
            currentTcpClient()->sendMessage(text);
        } else if(socketType == "UDP") {
    //        statusNetwork(currentTcpServer()->statusNetworkText);
        }
    }
    currentEditor()->plainText(QString(text).toUtf8()+"\r\n");
}

void KSocket::sendFile()
{
    QFileDialog fileDlg(this);
    fileDlg.setFileMode(QFileDialog::ExistingFile);
    QString filename;
    if (fileDlg.exec()) {
        QStringList fn = fileDlg.selectedFiles();
        if (!fn.isEmpty())
            filename = fn[0];
        else
            return;
    } else {
        return;
    }

    QFile fd(filename);
    if (!fd.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Opening file failed"), tr("Could not open file %1").arg(filename));
        return;
    }
    QByteArray data = fd.readAll();
    QProgressDialog* m_progress = new QProgressDialog(tr("Sending file..."), tr("Cancel"), 0, 100, this);
    m_progress->setMinimumDuration(100);
    int step = data.size() / 100;
    if (step < 1) {
        step = 1;
    }
    QString message;
    for (int i = 0; i < data.size(); i++) {
        if ((i % step) == 0) {
            m_progress->setValue(i / step);
            qApp->processEvents();
        }
        message.append(data.at(i));
        if (m_progress->wasCanceled())
            break;
    }

    if(socketType == "TCP Server") {
        currentTcpServer()->sendMessage(message);
    } else if(socketType == "TCP Client") {
        currentTcpClient()->sendMessage(message);
    } else if(socketType == "UDP") {
//        statusNetwork(currentTcpServer()->statusNetworkText);
    }

    delete m_progress;
    m_progress = nullptr;
    currentEditor()->plainText(QString("Send Success :" + filename).toUtf8());
}

void KSocket::saveToFile()
{
    QString Path = QFileDialog::getSaveFileName(nullptr, tr("Save File"), tr("Log (*.txt)"));
    if (Path!= nullptr) {
        QFileInfo(Path).dir().absolutePath();
        QFile FileOut(Path);
        FileOut.open(QIODevice::WriteOnly);
        if (FileOut.isOpen()) {
//            FileOut.write(currentSocket()->m_doc->text().toUtf8());
            FileOut.close();
        }
    }
}

void KSocket::closeProfile()
{
    if(socketType == "TCP Server") {
        delete currentTcpServer();
    } else if(socketType == "TCP Client") {
        delete currentUDP();
    }

    sockets.remove(tabWidget->currentWidget()->objectName().toInt());
    editors.removeOne(currentEditor());
    tabWidget->removeTab(tabWidget->currentIndex());
}

int KSocket::tabNumber()
{
    return tabWidget->currentIndex();
}

void KSocket::currectTabWidget(int index)
{
    currectSocket();
    if(socketType == "TCP Server") {
        statusNetwork(currentTcpServer()->statusNetworkText);
    } else if(socketType == "TCP Client") {
        statusNetwork(currentTcpClient()->statusNetworkText);
    } else if(socketType == "UDP") {
//        statusNetwork(currentTcpServer()->statusNetworkText);
    }
    currentUpdate();
}

KEditor* KSocket::currentEditor()
{
    return static_cast<KEditor*>(tabWidget->currentWidget());
}

QObject* KSocket::currectSocket()
{
    QMap<QString, QObject*> socketMap = sockets[tabWidget->currentWidget()->objectName().toInt()];
    QMap<QString, QObject*>::const_iterator it;
    QObject* itWidget = nullptr;
    for (it = socketMap.begin(); it != socketMap.cend(); ++it) {
        socketType = it.key();
        itWidget = static_cast<QObject*>(it.value());
    }
    return itWidget;
}

TCPServer* KSocket::currentTcpServer()
{
    return static_cast<TCPServer*>(currectSocket());
}

TCPClient* KSocket::currentTcpClient()
{
    return static_cast<TCPClient*>(currectSocket());
}

UDP* KSocket::currentUDP()
{
    return static_cast<UDP*>(currectSocket());
}

void KSocket::currentTabName(QString name)
{
    tabName = name;
}

}
