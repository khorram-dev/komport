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

#ifndef KSOCKET_H
#define KSOCKET_H

#include <QFileDialog>
#include <QListWidget>
#include <QTabWidget>

#include <KActionCollection>
#include <KSharedConfig>
#include <KConfigGroup>


#include <QWidget>
#include <QTcpSocket>
#include <QSettings>
#include <QTimer>
#include <QDateTime>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <KActionCollection>

#include <KTextEditor/Document>
#include <KTextEditor/View>
#include <KTextEditor/Editor>
#include <KMessageBox>

#include <interfaces/socketinterface.h>
#include "device/tcpserver.h"
#include "device/tcpclient.h"
#include "device/udp.h"
#include "newdevice.h"

namespace Komport {
class KListWidget;
class KEditor;
class KSocket : public SocketInterface
{
public:
    explicit KSocket(QWidget* parent=nullptr);
    ~KSocket() override;
//    socketGUI* currentSocket();
    void setupActions(KActionCollection* ac);
    void updateActions() override;
    int tabNumber();
    void reloadItems();
    KActionCollection* act;
    void sendText(QString text, int time) override;

public Q_SLOT:
    void showMenu();

private:
    QTabWidget* tabWidget;
    KListWidget *listWidget;
    newDevice *doDevice;
    KEditor* editor;
    QList<KEditor*> editors;
//    serialportGUI *serialportGui;
//    TSocket* socket;
//    QList<TSocket*> sockets;
//    socketGUI *socketGui;
//    QVector<socketGUI*> socketGuis;
//    QList<QAction*> column_action_list;
    QAction *pbOpen;
    QAction *cbNewLineAfter;
//    QAction *cbRowCount;
    QAction *cbTimestamp;
//    QAction *pbClear;
//    QAction *pbSelectAll;
//    QAction *pbCopy;
//    QAction *pbSaveToFile;
    QAction *pbSendFile;
    QAction *pbClose;
    QToolButton *terminal;
    QString messageTCPServer = "[TCP Server] ";
    QString messageTCPClient = "[TCP Client] ";
    QString messageUDPServer = "[UDP Server] ";
    QString messageUDPClient = "[UDP Client] ";

    void newFile();
    void openFile(const QString &fileName);

    void currentUpdate();
    void setMessageState();
    void saveToFile();
    void sendFile();
    void closeProfile();
    KEditor* currentEditor();

    void statusNetwork(const QString &statusName);

private slots:
//    void runProfile(int);
    void editItem(const QString &);
    void setLineMode(int);
    void changeTimer(int);











public:
//    TCPServer* tcpServer;
//    QList<TCPServer*> tcpServers;
//    TCPClient* tcpClient;
//    QList<TCPClient*> tcpClients;
//    UDP* udp;
//    QList<UDP*> udps;

    QMap<int, QMap<QString, QObject*>> sockets;

    QString fileName, ip, socketType, statusNetworkText="", BufferIn, backupBufferIn, bufferType, tabName;
    quint16 port = 0;

//    QPlainTextEdit *pteLog;
//    KTextEditor::View *m_view;
//    KTextEditor::Document *m_doc;
//    QPushButton *pbSend;
//    QLineEdit *leSend;
//    QComboBox *cbBuffer;
//    QSpinBox *sbNewLineAfterMs;
//    bool BNewLineAfter, BTimestamp;
//    bool isSuccessServer = false, isSuccessClient = false, isSuccessUDP = false;

//    void sendText(QString text, int time=0) override;
    void currentTabName(QString name);

//    QList<QAction *> actions;

public Q_SLOTS:
    void bufferText(const QString& text);
//    void sendFile(QString file);
    void applySettings();


private:
    QSettings *settings;
    QTimer *timer;
    QAction *statusBar;
    int setTime=1000;
    int COUNTER=0;
    void setupUi();
    void changeTimeer(int);
    void convertBufferIn(QByteArray &getText);
    void editorClear();
    void editorStatusBar();

    void currectTabWidget(int);
    QObject* currectSocket();
    TCPServer* currentTcpServer();
    TCPClient* currentTcpClient();
    UDP* currentUDP();

//Q_SIGNALS:
//    void statusNetwork(QString);
};
}
#endif // KSOCKET_H
