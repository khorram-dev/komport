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

#include "newdevice.h"
#include "settings.h"
#include "util/ksvg.h"

#include <QtDebug>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include <QSerialPortInfo>

namespace Komport {

newDevice::newDevice(QWidget *parent) : QWidget(parent)
{
    newSerialport();
    newSocket();
    installEventFilter(this);
}

newDevice::~newDevice() {}

void newDevice::newFile(QString device)
{
    if(device == "serialport") {
        dSerialport->exec();
    } else {
        dSocket->exec();
    }
}

void newDevice::findLocalIPs()
{
    comboBox_Interface->clear();
    interfaceList.clear();
    interfaceList = QNetworkInterface::allInterfaces();
    if (!interfaceList.isEmpty())
    {
        for (int j = 0; j < interfaceList.size(); ++j)
        {
            comboBox_Interface->addItem(interfaceList.at(j).humanReadableName());
        }
    }
    lineEdit_LocalIP->setText(interfaceList.at(0).addressEntries().at(0).ip().toString());
}

void newDevice::interfaceIndexChanged(int index)
{
    if(index > -1) {
        if (comboBox_Interface->count() > index && interfaceList.at(index).addressEntries().size() > 0)
        {
            for (int i = 0; i < interfaceList.at(index).addressEntries().size(); i++)
            {
                if (interfaceList.at(index).addressEntries().at(i).ip().protocol() == QAbstractSocket::IPv4Protocol)
                {
                    lineEdit_LocalIP->setText(interfaceList.at(index).addressEntries().at(i).ip().toString());
                }
            }
        } else {
            lineEdit_LocalIP->setText("255.255.255.255");
        }
    }
}

void newDevice::editProfile(QString device, QString path)
{
    QFile fileName(path+"/"+socketName->text());
    if(device == "serialport") {
        loadSerialport();
        dSerialport->exec();
        if(serialportName->isModified() && !serialportName->text().isEmpty()) {
            fileName.remove();
            Q_EMIT deviceReload();
        }
    } else {
        loadSocket();
        dSocket->exec();
        if(socketName->isModified() && !socketName->text().isEmpty()) {
            fileName.remove();
            Q_EMIT deviceReload();
        }
    }
}

void newDevice::saveSerialport()
{
    if(serialportName->text().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText(i18n("Profile Name should not be empty"));
        msgBox.exec();
        return;
    } else {
        QDir path(Settings::serialportDir());
        if(path.exists(serialportName->text())) {
            QMessageBox msgBox;
            msgBox.setText(i18n("The item has been replace."));
            msgBox.setInformativeText(i18n("Do you want to save your changes?"));
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();
            switch (ret) {
                case QMessageBox::Ok:
                {
                    path.rename(oldPath, serialportName->text());
                    Q_EMIT deviceReload();
                    break;
                }
                case QMessageBox::Cancel:
                break;
              }
        }

        QSettings *Settings= new QSettings(Settings::serialportDir()+"/"+serialportName->text(), QSettings::IniFormat);
        {
            Settings->beginGroup("Serialport");
            Settings->setValue("FileName", serialportName->text());
            Settings->setValue("Comport", cbDevicesPort->currentText());
            Settings->setValue("BaudRate", cbBaudRate->currentText().toInt());
            Settings->setValue("Parity", cbParity->currentText());
            Settings->setValue("DataBits", cbDataBits->currentText());
            Settings->setValue("StopBits", cbStopBits->currentText());
            Settings->setValue("FlowControl", cbFlowControl->currentText());
            Settings->setValue("SendBreak", cbSendBreak->isChecked());
        }{
            delete Settings;
        }
        dSerialport->accept();
    }
    Q_EMIT deviceReload();
}

void newDevice::saveSocket()
{
    if(socketName->text().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText(i18n("Profile Name should not be empty"));
        msgBox.exec();
        return;
    } else {
        QDir path(Settings::socketDir());
        if(path.exists(socketName->text())) {
            QMessageBox msgBox;
            msgBox.setText(i18n("The item has been replace."));
            msgBox.setInformativeText(i18n("Do you want to save your changes?"));
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();
            switch (ret) {
                case QMessageBox::Ok:
                {
                    path.rename(oldPath, socketName->text());
                    Q_EMIT deviceReload();
                    break;
                }
                case QMessageBox::Cancel:
                break;
              }
        }

        QSettings *Settings= new QSettings(Settings::socketDir()+"/"+socketName->text(), QSettings::IniFormat);
        {
            Settings->beginGroup("Socket");
            Settings->setValue("FileName", socketName->text());
            Settings->setValue("Type", comboBox_Interface_2->currentText());
            Settings->setValue("IP", lineEdit_LocalIP->text());
            Settings->setValue("PORT", lineEdit_UdpListenPort->text());
        }
        {
            delete Settings;
        }
        dSocket->accept();
    }
    Q_EMIT deviceReload();
}

void newDevice::loadSerialport()
{
    oldPath = serialportName->text();
    QSettings *settings= new QSettings(Settings::serialportDir()+"/"+serialportName->text(), QSettings::IniFormat);
    settings->beginGroup("Serialport");
//    leProfileName->setText("leProfileName->text()");
    cbDevicesPort->setCurrentText(settings->value("Comport").toString());
    cbBaudRate->setCurrentText(settings->value("BaudRate").toString());
    cbParity->setCurrentText(settings->value("Parity").toString());
    cbDataBits->setCurrentText(settings->value("DataBits").toString());
    cbStopBits->setCurrentText(settings->value("StopBits").toString());
    cbFlowControl->setCurrentText(settings->value("FlowControl").toString());
    cbSendBreak->setChecked(settings->value("SendBreak").toBool());
}

void newDevice::loadSocket()
{
    oldPath = socketName->text();
    QSettings *settings= new QSettings(Settings::socketDir()+"/"+socketName->text(), QSettings::IniFormat);
    settings->beginGroup("Socket");
    comboBox_Interface_2->setCurrentText(settings->value("Type").toString());
    lineEdit_LocalIP->setText(settings->value("IP").toString());
    lineEdit_UdpListenPort->setText(settings->value("PORT").toString());
}

void newDevice::openProfile()
{
    QString connectionName = QFileDialog::getOpenFileName(this, i18n("Open connection file"), QDir::currentPath());
    if (connectionName!= nullptr) {
        Q_EMIT openFile(connectionName);
    }
}

//bool newDevice::eventFilter(QObject *obj, QEvent *event)
//{
//  if (event->type() == QEvent::KeyPress) {
//      QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//      qDebug("Ate key press %d", keyEvent->key());
//      return true;
//  } else {
//      // standard event processing
//      return QObject::eventFilter(obj, event);
//  }
//}

//bool newDevice::eventFilter(QObject *watched, QEvent *event)
//{
//    if (event->type() == QEvent::MouseButtonPress ||
//            event->type() == QEvent::MouseButtonRelease ||
//            event->type() == QEvent::KeyRelease) {
//        qDebug() << "newDevice::eventFilter: " << event;
//    }

//    if(event->KeyPress == 16777217 || event->KeyPress == 16777218)
//        return true;
//    else
//        return QObject::eventFilter(watched, event);

////    return QObject::eventFilter(watched, event);
//}

void newDevice::newSerialport()
{
    dSerialport = new QDialog(this);
    if (dSerialport->objectName().isEmpty())
        dSerialport->setObjectName(QString::fromUtf8("Serialport"));
    dSerialport->resize(405, 378);
    verticalLayout = new QVBoxLayout(dSerialport);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    label = new QLabel(dSerialport);
    label->setObjectName(QString::fromUtf8("label"));

    horizontalLayout_3->addWidget(label);

    serialportName = new QLineEdit(dSerialport);
    serialportName->setObjectName(QString::fromUtf8("serialportName"));

    horizontalLayout_3->addWidget(serialportName);
    verticalLayout->addLayout(horizontalLayout_3);

    serialBox = new QGroupBox(dSerialport);
    serialBox->setObjectName(QString::fromUtf8("serialBox"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(1);
    sizePolicy.setHeightForWidth(serialBox->sizePolicy().hasHeightForWidth());
    serialBox->setSizePolicy(sizePolicy);
    gridLayout = new QGridLayout(serialBox);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(9, 9, 9, 9);
    laDataBits = new QLabel(serialBox);
    laDataBits->setObjectName(QString::fromUtf8("laDataBits"));

    gridLayout->addWidget(laDataBits, 4, 0, 1, 1);

    laStopBits = new QLabel(serialBox);
    laStopBits->setObjectName(QString::fromUtf8("laStopBits"));

    gridLayout->addWidget(laStopBits, 5, 0, 1, 1);

    laBaudRate = new QLabel(serialBox);
    laBaudRate->setObjectName(QString::fromUtf8("laBaudRate"));

    gridLayout->addWidget(laBaudRate, 2, 0, 1, 1);

    cbBaudRate = new QComboBox(serialBox);
    cbBaudRate->setObjectName(QString::fromUtf8("cbBaudRate"));

    gridLayout->addWidget(cbBaudRate, 2, 1, 1, 1);

    laParity = new QLabel(serialBox);
    laParity->setObjectName(QString::fromUtf8("laParity"));

    gridLayout->addWidget(laParity, 3, 0, 1, 1);

    cbParity = new QComboBox(serialBox);
    cbParity->setObjectName(QString::fromUtf8("cbParity"));

    gridLayout->addWidget(cbParity, 3, 1, 1, 1);

    laDevicesPort = new QLabel(serialBox);
    laDevicesPort->setObjectName(QString::fromUtf8("laDevicesPort"));

    gridLayout->addWidget(laDevicesPort, 1, 0, 1, 1);

    cbStopBits = new QComboBox(serialBox);
    cbStopBits->setObjectName(QString::fromUtf8("cbStopBits"));

    gridLayout->addWidget(cbStopBits, 5, 1, 1, 1);

    cbFlowControl = new QComboBox(serialBox);
    cbFlowControl->setObjectName(QString::fromUtf8("cbFlowControl"));

    gridLayout->addWidget(cbFlowControl, 6, 1, 1, 1);

    cbSendBreak = new QCheckBox(serialBox);
    cbSendBreak->setObjectName(QString::fromUtf8("cbSendBreak"));

    gridLayout->addWidget(cbSendBreak, 7, 1, 1, 1);

    laFlowControl = new QLabel(serialBox);
    laFlowControl->setObjectName(QString::fromUtf8("laFlowControl"));

    gridLayout->addWidget(laFlowControl, 6, 0, 1, 1);

    laSendBreak = new QLabel(serialBox);
    laSendBreak->setObjectName(QString::fromUtf8("laSendBreak"));

    gridLayout->addWidget(laSendBreak, 7, 0, 1, 1);

    cbDataBits = new QComboBox(serialBox);
    cbDataBits->setObjectName(QString::fromUtf8("cbDataBits"));

    gridLayout->addWidget(cbDataBits, 4, 1, 1, 1);

    cbDevicesPort = new QComboBox(serialBox);
    cbDevicesPort->setObjectName(QString::fromUtf8("cbDevicesPort"));

    gridLayout->addWidget(cbDevicesPort, 1, 1, 1, 1);

    line_2 = new QFrame(serialBox);
    line_2->setObjectName(QString::fromUtf8("line_2"));
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);

    gridLayout->addWidget(line_2, 0, 1, 1, 1);

    line_3 = new QFrame(serialBox);
    line_3->setObjectName(QString::fromUtf8("line_3"));
    line_3->setFrameShape(QFrame::HLine);
    line_3->setFrameShadow(QFrame::Sunken);

    gridLayout->addWidget(line_3, 0, 0, 1, 1);
    verticalLayout->addWidget(serialBox);

    widget = new QWidget(dSerialport);
    widget->setObjectName(QString::fromUtf8("widget"));
    horizontalLayout = new QHBoxLayout(widget);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(1, 1, 1, 1);
    horizontalSpacer = new QSpacerItem(250, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    QPushButton* pbOK = new QPushButton(widget);
    pbOK->setObjectName(QString::fromUtf8("pbOK"));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/Icons/Icons/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
    pbOK->setIcon(icon);

    horizontalLayout->addWidget(pbOK);

    QPushButton* pbCancel = new QPushButton(widget);
    pbCancel->setObjectName(QString::fromUtf8("pbCancel"));

    horizontalLayout->addWidget(pbCancel);
    verticalLayout->addWidget(widget);

    QWidget::setTabOrder(cbDevicesPort, cbBaudRate);
    QWidget::setTabOrder(cbBaudRate, cbParity);
    QWidget::setTabOrder(cbParity, cbDataBits);
    QWidget::setTabOrder(cbDataBits, cbStopBits);
    QWidget::setTabOrder(cbStopBits, cbFlowControl);
    QWidget::setTabOrder(cbFlowControl, cbSendBreak);
    QWidget::setTabOrder(cbSendBreak, pbOK);

    setWindowTitle(tr2i18n("Open Com Port", nullptr));
    label->setText(tr2i18n("Profile name:", nullptr));
    serialBox->setTitle(tr2i18n("RS232", nullptr));
    laDataBits->setText(tr2i18n("Data bits", nullptr));
    laStopBits->setText(tr2i18n("Stop bits", nullptr));
    laBaudRate->setText(tr2i18n("Baud rate", nullptr));
    laParity->setText(tr2i18n("Parity", nullptr));
    laDevicesPort->setText(tr2i18n("Com Port", nullptr));
    laFlowControl->setText(tr2i18n("Flow control", nullptr));
    laSendBreak->setText(tr2i18n("Send Break", nullptr));
    pbOK->setText(tr2i18n("Ok", nullptr));
    pbCancel->setText(tr2i18n("Cancel", nullptr));

    pbOK->setDefault(true);

    cbBaudRate->addItem("1200");
    cbBaudRate->addItem("2400");
    cbBaudRate->addItem("4800");
    cbBaudRate->addItem("9600");
    cbBaudRate->addItem("19200");
    cbBaudRate->addItem("38400");
    cbBaudRate->addItem("57600");
    cbBaudRate->addItem("115200");
    cbBaudRate->setCurrentIndex(3);
    cbParity->addItem("None");
    cbParity->addItem("Even");
    cbParity->addItem("Odd");
    cbParity->addItem("Mark");
    cbParity->addItem("Space");
    cbParity->setCurrentIndex(0);
    cbDataBits->addItem("5");
    cbDataBits->addItem("6");
    cbDataBits->addItem("7");
    cbDataBits->addItem("8");
    cbDataBits->setCurrentIndex(3);
    cbStopBits->addItem("1");
    cbStopBits->addItem("1.5");
    cbStopBits->addItem("2");
    cbStopBits->setCurrentIndex(0);
    cbFlowControl->addItem("None");
    cbFlowControl->addItem("RTS/CTS");
    cbFlowControl->addItem("XON/XOFF");
    cbFlowControl->setCurrentIndex(0);

    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        #ifdef Q_OS_LINUX
        cbDevicesPort->addItem("/dev/"+info.portName());
        #endif
        #ifdef Q_OS_MACOS
        cbDevicesPort->addItem(info.portName());
        #endif
        #ifdef Q_OS_WIN
        cbDevicesPort->addItem(info.portName());
        #endif
    }
    cbDevicesPort->setEditable(true);
    connect(pbOK, &QPushButton::clicked, this, &newDevice::saveSerialport);
    connect(pbCancel, &QPushButton::clicked, dSerialport, &QDialog::reject);
    QMetaObject::connectSlotsByName(dSerialport);
}

void newDevice::newSocket()
{
    dSocket = new QDialog(this);
    if (dSocket->objectName().isEmpty())
        dSocket->setObjectName(QString::fromUtf8("Socket"));
    dSocket->resize(405, 378);
    verticalLayout = new QVBoxLayout(dSocket);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    label = new QLabel(dSocket);
    label->setObjectName(QString::fromUtf8("label"));
    horizontalLayout_3->addWidget(label);
    socketName = new QLineEdit(dSocket);
    socketName->setObjectName(QString::fromUtf8("socketName"));
    horizontalLayout_3->addWidget(socketName);
    verticalLayout->addLayout(horizontalLayout_3);
    groupBox_3 = new QGroupBox(dSocket);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    verticalLayout_2 = new QVBoxLayout(groupBox_3);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    line = new QFrame(groupBox_3);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    verticalLayout_2->addWidget(line);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    comboBox_Interface = new QComboBox(groupBox_3);
    comboBox_Interface->setObjectName(QString::fromUtf8("comboBox_Interface"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(comboBox_Interface->sizePolicy().hasHeightForWidth());
    comboBox_Interface->setSizePolicy(sizePolicy);
    comboBox_Interface->setMinimumSize(QSize(100, 0));
    comboBox_Interface->setMaximumSize(QSize(200, 16777215));
    comboBox_Interface->setEditable(false);

    horizontalLayout_2->addWidget(comboBox_Interface);

    label_2 = new QLabel(groupBox_3);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
    label_2->setSizePolicy(sizePolicy1);

    horizontalLayout_2->addWidget(label_2);

    lineEdit_LocalIP = new QLineEdit(groupBox_3);
    lineEdit_LocalIP->setObjectName(QString::fromUtf8("lineEdit_LocalIP"));

    horizontalLayout_2->addWidget(lineEdit_LocalIP);

    horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(horizontalSpacer_5);

    button_Refresh = new QPushButton(groupBox_3);
    button_Refresh->setObjectName(QString::fromUtf8("button_Refresh"));

    horizontalLayout_2->addWidget(button_Refresh);
    verticalLayout_2->addLayout(horizontalLayout_2);

    horizontalLayout_4 = new QHBoxLayout();
    horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
    comboBox_Interface_2 = new QComboBox(groupBox_3);
    comboBox_Interface_2->addItem(QString());
    comboBox_Interface_2->addItem(QString());
    comboBox_Interface_2->addItem(QString());
    comboBox_Interface_2->setObjectName(QString::fromUtf8("comboBox_Interface_2"));
    sizePolicy.setHeightForWidth(comboBox_Interface_2->sizePolicy().hasHeightForWidth());
    comboBox_Interface_2->setSizePolicy(sizePolicy);
    comboBox_Interface_2->setMinimumSize(QSize(100, 0));
    comboBox_Interface_2->setMaximumSize(QSize(200, 16777215));

    horizontalLayout_4->addWidget(comboBox_Interface_2);

    label_3 = new QLabel(groupBox_3);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
    label_3->setSizePolicy(sizePolicy1);

    horizontalLayout_4->addWidget(label_3);

    lineEdit_UdpListenPort = new QLineEdit(groupBox_3);
    lineEdit_UdpListenPort->setObjectName(QString::fromUtf8("lineEdit_UdpListenPort"));
    sizePolicy1.setHeightForWidth(lineEdit_UdpListenPort->sizePolicy().hasHeightForWidth());
    lineEdit_UdpListenPort->setSizePolicy(sizePolicy1);
    lineEdit_UdpListenPort->setMinimumSize(QSize(80, 0));
    lineEdit_UdpListenPort->setMaximumSize(QSize(80, 16777215));

    horizontalLayout_4->addWidget(lineEdit_UdpListenPort);

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_4->addItem(horizontalSpacer_2);
    verticalLayout_2->addLayout(horizontalLayout_4);

    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_2->addItem(verticalSpacer);
    verticalLayout->addWidget(groupBox_3);

    widget = new QWidget(dSocket);
    widget->setObjectName(QString::fromUtf8("widget"));
    horizontalLayout = new QHBoxLayout(widget);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(1, 1, 1, 1);
    horizontalSpacer = new QSpacerItem(250, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    pbOK = new QPushButton(widget);
    pbOK->setObjectName(QString::fromUtf8("pbOK"));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/Icons/Icons/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
    pbOK->setIcon(icon);

    horizontalLayout->addWidget(pbOK);

    pbCancel = new QPushButton(widget);
    pbCancel->setObjectName(QString::fromUtf8("pbCancel"));

    horizontalLayout->addWidget(pbCancel);
    verticalLayout->addWidget(widget);

    setWindowTitle(tr2i18n("Open Com Port", nullptr));
    label->setText(tr2i18n("Profile name:", nullptr));
    groupBox_3->setTitle(tr2i18n("Network Interface:", nullptr));
    label_2->setText(tr2i18n("IP: ", nullptr));
    button_Refresh->setText(tr2i18n("Refresh", nullptr));
    comboBox_Interface_2->setItemText(0, tr2i18n("TCP Client", nullptr));
    comboBox_Interface_2->setItemText(1, tr2i18n("TCP Server", nullptr));
    comboBox_Interface_2->setItemText(2, tr2i18n("UDP", nullptr));

    label_3->setText(tr2i18n("Port:", nullptr));
    pbOK->setText(tr2i18n("Ok", nullptr));
    pbCancel->setText(tr2i18n("Cancel", nullptr));

    pbOK->setDefault(true);

    findLocalIPs();
    connect(button_Refresh, &QPushButton::clicked, this, &newDevice::findLocalIPs);
    connect(comboBox_Interface, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this , &newDevice::interfaceIndexChanged);

    connect(pbOK, &QPushButton::clicked, this, &newDevice::saveSocket);
    connect(pbCancel, &QPushButton::clicked, dSocket, &QDialog::reject);

    QMetaObject::connectSlotsByName(this);
}
}
