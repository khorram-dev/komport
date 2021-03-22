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

#ifndef TSERIALPORT_H
#define TSERIALPORT_H

#include <QObject>
#include <QTcpSocket>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <QDateTime>
#include <QSettings>
#include <QMessageBox>
#include <QList>
#include <interfaces/serialportinterface.h>
#include <libkomport_export.h>

namespace Komport {

class LIBKOMPORT_EXPORT TSerialport : public QWidget
{
    Q_OBJECT

public:
    explicit TSerialport(QWidget *parent = nullptr);
    explicit TSerialport(QString name, int baudRate, int dataBits, int parity, int stopBits, int flowControl, bool sendBreak, QWidget *parent = nullptr);// :‌ QWidget(parent){}
//    explicit TSerialport(QString name, int baudRate, int dataBits, int parity, int stopBits, int flowControl);
    ~TSerialport();
    bool openPort();
    bool setDevice(QString name, int baudRate, int dataBits, int parity, int stopBits, int flowControl, bool sendBreak);
    void breakEnabled(bool);

    QString BufferIn, backupBufferIn, bufferType;
    int TabId;
//    QChar serialError;
//    void openDevice();

    bool status();
    bool isDTR();
    bool isRTS();
    void setDTR(bool type);
    void setRTS(bool type);
//    void setpinoutSignals();

//    void sendByteArray(QByteArray QBABufferIn);
//    void logClear();
    bool sendTextTimer(const char c, int delay);
//    void logFile();
    void sendFile(QString fileName);
//    void readSerial();

    enum LineMode { NONE = 0, LF, CR, CRLF, HEX };
    Q_ENUMS(LineMode)

    enum Protocol { PLAIN, SCRIPT, XMODEM, YMODEM, ZMODEM, ONEKXMODEM, PROTOCOL_MAX };
    Q_ENUMS(Protocol)

    int timedelay=1000;
    bool isLogFile=false;

public Q_SLOT:
//    void reloadPort();
    void sendText(QString text);
    void setLineMode(int x);
    void changeTimer(int);
    void logStart(bool start);
    void closeDevice();

private:
    QSerialPort *serialport;
    QTimer *timer;
    QFile logFile;


    QString s_name;
    int s_baudRate;
    int s_dataBits;
    int s_parity;
    int s_stopBits;
    int s_flowControl;
    bool s_sendbreak;

    LineMode lineMode;

private Q_SLOTS:
    void readData();
    bool sendString(QString *s);
//    void writeData(const QByteArray &data);
    void sendTimer();
    void handleError(QSerialPort::SerialPortError error);


Q_SIGNALS:
    void sendData(QByteArray data);
//    void serialSignals(QSerialPort::PinoutSignals);
//    void sendHandleError(const QString& error);

};
}
#endif // TSERIALPORT_H
