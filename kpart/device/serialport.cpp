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

#include "serialport.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <KLocalizedString>

namespace Komport {
void millisleep(unsigned long ms)
{
    if (ms > 0) {
        QThread::msleep(ms);
    }
}

TSerialport::TSerialport(QWidget *parent)
{
    serialport = new QSerialPort(parent);
    timer = new QTimer(parent);
    lineMode = LineMode::NONE;
}

TSerialport::TSerialport(QString name, int baudRate, int dataBits, int parity, int stopBits, int flowControl, bool sendBreak, QWidget* parent) : QWidget(parent), s_name(name), s_baudRate(baudRate), s_dataBits(dataBits), s_parity(parity), s_stopBits(stopBits), s_flowControl(flowControl), s_sendbreak(sendBreak)
{
//    serialports.insert(0, new QSerialPort);
//    timers.insert(0,new QTimer);
    serialport = new QSerialPort(this);
    serialport->setObjectName(name);
//    serialports.append(serialport);
    timer = new QTimer(this);
    timer->setObjectName(name);
//    timers.append(timer);

    lineMode = LineMode::NONE;
}

TSerialport::~TSerialport(){}

bool TSerialport::setDevice(QString name, int baudRate, int dataBits, int parity, int stopBits, int flowControl, bool sendBreak)
{
    s_name = name;
    s_baudRate = baudRate;
    s_dataBits = dataBits;
    s_parity = parity;
    s_stopBits = stopBits;
    s_flowControl = flowControl;
    s_sendbreak = sendBreak;
    serialport->setPortName(s_name);
//    connect(serialport, &QSerialPort::errorOccurred, this, &TSerialport::handleError);
    if (serialport->open(QIODevice::ReadWrite)) {
        connect(serialport, &QSerialPort::readyRead, this, &TSerialport::readData);
        bool BOOLDataBits, BOOLParity, BOOLStopBits, BOOLFlowControl;
        BOOLDataBits = BOOLParity = BOOLStopBits = BOOLFlowControl= false;

        serialport->setBaudRate(static_cast<quint16>(s_baudRate));
        BOOLDataBits = serialport->setDataBits(static_cast<QSerialPort::DataBits>(s_dataBits));
        BOOLParity = serialport->setParity(static_cast<QSerialPort::Parity>(s_parity));
        BOOLStopBits = serialport->setStopBits(static_cast<QSerialPort::StopBits>(s_stopBits));
        BOOLFlowControl = serialport->setFlowControl(static_cast<QSerialPort::FlowControl>(s_flowControl));

        if (BOOLDataBits && BOOLParity && BOOLStopBits && BOOLFlowControl) {
            serialport->setDataTerminalReady(false);
            serialport->setRequestToSend(false);
            connect(timer, &QTimer::timeout, this, &TSerialport::sendTimer);
            timer->start(timedelay);
            breakEnabled(s_sendbreak);
        } else {
            handleError(serialport->error());
            serialport->close();
            return false;
        }
        return true;
    } else {
        handleError(serialport->error());
        serialport->close();
        return false;
    }
}

bool TSerialport::openPort()
{
    serialport->setPortName(s_name);
//    connect(serialport, &QSerialPort::errorOccurred, this, &TSerialport::handleError);
    if (serialport->open(QIODevice::ReadWrite)) {
        connect(serialport, &QSerialPort::readyRead, this, &TSerialport::readData);
        bool BOOLDataBits, BOOLParity, BOOLStopBits, BOOLFlowControl;
        BOOLDataBits = BOOLParity = BOOLStopBits = BOOLFlowControl= false;
        serialport->setBaudRate(static_cast<quint16>(s_baudRate));
        BOOLDataBits = serialport->setDataBits(static_cast<QSerialPort::DataBits>(s_dataBits));
        BOOLParity = serialport->setParity(static_cast<QSerialPort::Parity>(s_parity));
        BOOLStopBits = serialport->setStopBits(static_cast<QSerialPort::StopBits>(s_stopBits));
        BOOLFlowControl = serialport->setFlowControl(static_cast<QSerialPort::FlowControl>(s_flowControl));
        if (BOOLDataBits && BOOLParity && BOOLStopBits && BOOLFlowControl) {
            serialport->setDataTerminalReady(false);
            serialport->setRequestToSend(false);
            timer = new QTimer(this);
            connect(timer, &QTimer::timeout, this, &TSerialport::sendTimer);
            timer->start(timedelay);
        } else {
            handleError(serialport->error());
            serialport->close();
            return false;
        }
        return true;
    } else {
        handleError(serialport->error());
        serialport->close();
        return false;
    }
}

//void TSerialport::reloadPort()
//{
//    qDebug() << "reloadPort..." << Qt::endl;
//    if(serialport->isOpen()) {
//        QMessageBox msgBox;
//        msgBox.setText("The service has been connected.");
//        msgBox.setInformativeText("Do you want to reload your device?");
//        msgBox.setStandardButtons(QMessageBox::Apply | QMessageBox::Cancel);
//        msgBox.setDefaultButton(QMessageBox::Apply);
//        int ret = msgBox.exec();
//        switch (ret) {
//        case QMessageBox::Apply:
//            closeDevice();
//            openPort();
//            break;
//        case QMessageBox::Cancel:
//            break;
//        default:
//            // should never be reached
//            break;
//        }
//    }
//}

void TSerialport::breakEnabled(bool tf)
{
    serialport->setBreakEnabled(tf);
}

void TSerialport::readData()
{
    QByteArray data = serialport->readAll();
    if (isLogFile) {
        logFile.write(data);
        logFile.flush();
    }

    Q_EMIT sendData(data);
}

void TSerialport::sendText(QString text)
{
    sendString(&text);
    if (isLogFile) {
        logFile.write(text.toUtf8()+"\r\n");
        logFile.flush();
    }
}

bool TSerialport::sendString(QString *s)
{
    if (lineMode == LineMode::HEX)
    {
        QString hex = *s;
        hex.remove(QRegExp("\\s+(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)")); // spaces except that in quotes
        if ((hex.startsWith("0x")) || (hex.startsWith("0X"))) {
            hex = hex.mid(2);
        }

        bool ascii = false;
        for (int i = 0; i < hex.length();) {
            QString nextByte = hex.mid(i, ascii ? 1 : 2);
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
                QMessageBox::information(this, tr("Invalid hex characters"),
                                         tr("The input string contains invalid hex characters: 0x%1").arg(nextByte));
                return false;
            }
        }

        if (ascii) {
            QMessageBox::information(this, tr("Invalid format"), tr("No closing quote"));
            return false;
        }

        for (int i = 0; i < hex.length();) {
            QString nextByte = hex.mid(i, ascii ? 1 : 2);
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

            sendTextTimer(static_cast<char>(byte & 0xff), timer->interval());
        }
        return true;
    } else {
        // converts QString into QByteArray, this supports converting control characters being shown in input field as
        // QChars
        // of Control Pictures from Unicode block.
        QByteArray bytes;
        bytes.reserve(s->size());
        for (auto &c : *s) {
            bytes.append(static_cast<char>(c.unicode()));
        }
        for (int i = 0; i < bytes.length(); i++) {
            if (!sendTextTimer(bytes[i], timer->interval()))
                return false;
        }
        switch (lineMode) {
        case LineMode::LF:
            if (!sendTextTimer('\n', timer->interval()))
                return false;
            break;
        case LineMode::CR:
            if (!sendTextTimer('\r', timer->interval()))
                return false;
            break;
        case LineMode::CRLF:
            if (!sendTextTimer('\r', timer->interval()))
                return false;
            if (!sendTextTimer('\n', timer->interval()))
                return false;
            break;
        default:
            break;
        }
    }
    return false;
}

bool TSerialport::sendTextTimer(const char c, int delay)
{
    if (!serialport->isOpen()) {
        return false;
    }
    if ((serialport->write(&c, 1)) < 1) {
        qDebug() << serialport->errorString();
        return false;
    }
    serialport->waitForBytesWritten(timer->interval());

    if (delay) {
        timer->setInterval(delay);
        serialport->flush();
    }
    return true;
}

void TSerialport::sendTimer()
{
    timer->stop();
//    setpinoutSignals();
//    Q_EMIT serialSignals(serialport->pinoutSignals());
    QByteArray QBABufferIn = serialport->readAll();
    if (QBABufferIn.size() > 0) {
        sendString(new QString(QBABufferIn));
    }
    timer->start(timer->interval());
}

bool TSerialport::isDTR()
{
    return serialport->isDataTerminalReady();
}

bool TSerialport::isRTS()
{
    return serialport->isRequestToSend();
}

void TSerialport::setDTR(bool type)
{
    serialport->setDataTerminalReady(type);
}

void TSerialport::setRTS(bool type)
{
    serialport->setRequestToSend(type);
}

//void TSerialport::setpinoutSignals()
//{
//    serialport->pinoutSignals();
//}

void TSerialport::sendFile(QString fileName)
{
    QFile fileIn(fileName);
    if (fileIn.open(QIODevice::ReadOnly)) {
        QByteArray QBABufferIn = fileIn.readAll();
        if (serialport->isOpen()) {
//            if (SendBreak) {
//                serialport->setBreakEnabled(true);
//                serialport->setBreakEnabled(false);
//            }
            serialport->write(QBABufferIn, QBABufferIn.size());
            serialport->waitForBytesWritten(2000);
        }
        fileIn.close();
    }
}

void TSerialport::changeTimer(int x)
{
    timedelay = x;
    timer->setInterval(timedelay);
}

void TSerialport::setLineMode(int x)
{
    lineMode = static_cast<LineMode>(x);
}

bool TSerialport::status()
{
    return serialport->isOpen();
}

void TSerialport::logStart(bool start)
{
    if(start) {
        QString logFilePath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)+QStringLiteral("/log");
        QString logFileName = "/" + /*s_name + "_" +*/ QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss")+".txt";
        QDir dir;
        if (!dir.exists(logFilePath))
            dir.mkdir(logFilePath);
        logFile.setFileName(logFilePath+logFileName);
        QTextStream out(&logFile);
        out << "[Komport log] " + s_name + "\n";
        if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::information(this, tr("Opening file failed"),
                                     tr("Could not open file %1 for writing").arg(logFileName));
        }
        isLogFile = start;
    } else {
        logFile.close();
        isLogFile = start;
    }

}

void TSerialport::handleError(QSerialPort::SerialPortError error)
{
//    int x = error;
//    qDebug() << "TSerialport:handleError: " << serialport->errorString() << " :‌ " << error << Qt::endl;
    QMessageBox msgBox;
        switch (error) {
            case 0:
                msgBox.warning(this, i18n("Error"), i18n("No error occurred."));
                break;
            case 1:
                msgBox.warning(this, "Error", i18n("An error occurred while attempting to open an non-existing device."));
                break;
            case 2:
                msgBox.warning(this, "Error", i18n("An error occurred while attempting to open an already opened device by another process or a user not having enough permission and credentials to open."));
                break;
            case 3:
                msgBox.warning(this, "Error", i18n("An error occurred while attempting to open an already opened device in this object."));
                break;
            case 4:
                msgBox.warning(this, "Error", i18n("Parity error detected by the hardware while reading data. This value is obsolete. We strongly advise against using it in new code."));
                break;
            case 5:
                msgBox.warning(this, "Error", i18n("Framing error detected by the hardware while reading data. This value is obsolete. We strongly advise against using it in new code."));
                break;
            case 6:
                msgBox.warning(this, "Error", i18n("Break condition detected by the hardware on the input line. This value is obsolete. We strongly advise against using it in new code."));
                break;
            case 7:
                msgBox.warning(this, "Error", i18n("An I/O error occurred while writing the data."));
                break;
            case 8:
                msgBox.warning(this, "Error", i18n("An I/O error occurred while reading the data."));
                break;
            case 9:
                msgBox.warning(this, "Error", i18n("An I/O error occurred when a resource becomes unavailable, e.g. when the device is unexpectedly removed from the system."));
                break;
            case 10:
                msgBox.warning(this, "Error", i18n("The requested device operation is not supported or prohibited by the running operating system."));
                break;
            case 11:
                msgBox.warning(this, "Error", i18n("An unidentified error occurred."));
                break;
            case 12:
                msgBox.warning(this, "Error", i18n("A timeout error occurred. This value was introduced in QtSerialPort 5.2."));
                break;
            case 13:
                msgBox.warning(this, "Error", i18n("This error occurs when an operation is executed that can only be successfully performed if the device is open. This value was introduced in QtSerialPort 5.2."));
                break;
            }
}

void TSerialport::closeDevice()
{
    if (isLogFile) {
        logFile.flush();
        logFile.close();
    }
    if(serialport->isOpen()) {
        disconnect(timer, &QTimer::timeout, this, &TSerialport::sendTimer);
        delete timer;
        serialport->clearError();
        serialport->close();
    }
}

}
