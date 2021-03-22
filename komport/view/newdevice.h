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

#ifndef NEWDEVICE_H
#define NEWDEVICE_H

#include <QVariant>
#include <QIcon>
#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QSpacerItem>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <QDialog>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QKeyEvent>
#include <KActionCollection>
#include <KLocalizedString>
//#include "ui_newdevice.h"

#define TCPSERVER 10
#define TCPCLIENT 20
#define UDPSERVER 30
#define APPVERSION "V1.2"

namespace Komport {

class newDevice : public QWidget//, public Ui::newDevice
{
    Q_OBJECT

public:
    explicit newDevice(QWidget *parent = nullptr);
    ~newDevice();
    void editProfile(QString device, QString path);


    void openProfile();
    void newFile(QString device);

    QLineEdit *serialportName;
    QLineEdit *socketName;

private slots:
    void interfaceIndexChanged(int index);
    void saveSerialport();
    void saveSocket();

Q_SIGNALS:
    void openFile(QString&);
    void deviceReload();

//protected:
//    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QList<QNetworkInterface> interfaceList;
    QString oldPath;
    void findLocalIPs();
    void newSerialport();
    void newSocket();
    void loadSerialport();
    void loadSocket();
    QDialog *dSerialport;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QGroupBox *serialBox;
    QGridLayout *gridLayout;
    QLabel *laDataBits;
    QLabel *laStopBits;
    QLabel *laBaudRate;
    QComboBox *cbBaudRate;
    QLabel *laParity;
    QComboBox *cbParity;
    QLabel *laDevicesPort;
    QComboBox *cbStopBits;
    QComboBox *cbFlowControl;
    QCheckBox *cbSendBreak;
    QLabel *laFlowControl;
    QLabel *laSendBreak;
    QComboBox *cbDataBits;
    QComboBox *cbDevicesPort;
    QFrame *line_2;
    QFrame *line_3;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbOK;
    QPushButton *pbCancel;

    QDialog *dSocket;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_2;
    QFrame *line;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *comboBox_Interface;
    QLabel *label_2;
    QLineEdit *lineEdit_LocalIP;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *button_Refresh;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *comboBox_Interface_2;
    QLabel *label_3;
    QLineEdit *lineEdit_UdpListenPort;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;


};
}
#endif // NEWDEVICE_H
