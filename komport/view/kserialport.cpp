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


#include "kserialport.h"
#include <QSettings>
#include <QMenu>
#include <QVBoxLayout>
#include <QLayout>
#include <QThread>
#include <QProgressDialog>
#include <KLocalizedString>
#include <settings.h>
#include "statusbar.h"
#include "util/ksvg.h"
#include "pref/generalpref.h"
#include "gui/klistwidget.h"
#include "editor.h"

namespace Komport {

void millisleep(unsigned long ms)
{
    if (ms > 0) {
        QThread::msleep(ms);
    }
}

KSerialport::KSerialport(QWidget *parent) : SerialportInterface(i18n("Serialport"), ":/images/svg/serialport.svg", parent)
{
    setXMLGUIFile(QStringLiteral("serialportui.rc"));
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
//    editors.append(new KEditor);
//    serialports.insert(0, new TSerialport);
    doDevice = new newDevice(parent);
    connect(doDevice, &newDevice::openFile, this, &KSerialport::openFile);
    connect(doDevice, &newDevice::deviceReload, listWidget, &KListWidget::readItems);
    connect(tabWidget, &QTabWidget::currentChanged, this, &KSerialport::showMenu);
    connect(tabWidget, &QTabWidget::currentChanged, this, &KSerialport::currentUpdate);
    connect(listWidget, &KListWidget::runFile, this, &KSerialport::openFile);
    connect(listWidget, &KListWidget::editItem, this, &KSerialport::editItem);
    setLayout(mlayout);
}

KSerialport::~KSerialport(){}

void KSerialport::reloadItems()
{
    if(Settings::serialportDir().isEmpty())
        listWidget->setPath(qApp->applicationDirPath()+"/serialport");
    else
        listWidget->setPath(Settings::serialportDir());
    listWidget->readItems();
}

void KSerialport::setupActions(KActionCollection* ac)
{
//    Action newDevice
    QAction *newProfile = new QAction(Ksvg::seticonSVG(":/images/svg/document-new.svg", Settings::iconColor()), i18n("New"), this);
    newProfile->setToolTip(i18n("New Serialport"));
    connect(newProfile, &QAction::triggered, this, &KSerialport::newFile);
    ac->addAction(QStringLiteral("ac_new"), newProfile);
    ac->setDefaultShortcut(newProfile, QKeySequence(Qt::CTRL + Qt::Key_N));

    QAction *openProfile = new QAction(Ksvg::seticonSVG(":/images/svg/document-open.svg", Settings::iconColor()), i18n("Open"), this);
    openProfile->setToolTip(i18n("Open Serialport"));
    connect(openProfile, &QAction::triggered, doDevice, &newDevice::openProfile);
    ac->addAction(QStringLiteral("ac_open"), openProfile);
    ac->setDefaultShortcut(openProfile, QKeySequence(Qt::CTRL + Qt::Key_O));

//    Action serialportGUI
    pbOpen = new QAction(Ksvg::seticonSVG(":/images/svg/plug_connect.svg", Settings::iconColor()), i18n("Connect"), this);
	pbOpen->setToolTip(i18n("Connect in the current tab"));
    pbOpen->setEnabled(false);
    ac->addAction(QStringLiteral("ac_connection"), pbOpen);
    ac->setDefaultShortcut(pbOpen, QKeySequence(Qt::CTRL + Qt::Key_Enter));

    pbRTS = new QAction(Ksvg::seticonSVG(":/images/svg/RTS.svg", Settings::iconColor()), i18n("RTS"), this);
	pbRTS->setToolTip(i18n("Request to Send"));
    pbRTS->setEnabled(false);
    pbRTS->setCheckable(true);
    pbRTS->setChecked(false);
    ac->addAction(QStringLiteral("ac_rts"), pbRTS);
//    ac->setDefaultShortcut(pbRTS, QKeySequence(Qt::CTRL + Qt::Key_M));

    pbDTR = new QAction(Ksvg::seticonSVG(":/images/svg/DTR.svg", Settings::iconColor()), i18n("DTR"), this);
	pbDTR->setToolTip(i18n("Data Set Ready"));
    pbDTR->setEnabled(false);
    pbDTR->setCheckable(true);
    pbDTR->setChecked(false);
    ac->addAction(QStringLiteral("ac_dtr"), pbDTR);
//    ac->setDefaultShortcut(pbDTR, QKeySequence(Qt::CTRL + Qt::Key_M));

    newLineAfter = new QAction(Ksvg::seticonSVG(":/images/svg/newline.svg", Settings::iconColor()), i18n("New Line"), this);
    newLineAfter->setToolTip(i18n("New line each time"));
    newLineAfter->setEnabled(false);
    newLineAfter->setCheckable(true);
    newLineAfter->setChecked(true);
    ac->addAction(QStringLiteral("ac_newlineafter"), newLineAfter);
    ac->setDefaultShortcut(newLineAfter, QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N));

    timeStamp = new QAction(Ksvg::seticonSVG(":/images/svg/time-left.svg", Settings::iconColor()), i18n("Timestamp"), this);
    timeStamp->setToolTip(i18n("Append Time & Date in terminal"));
    timeStamp->setEnabled(false);
    timeStamp->setCheckable(true);
    timeStamp->setChecked(false);
    ac->addAction(QStringLiteral("ac_timestamp"), timeStamp);
    ac->setDefaultShortcut(timeStamp, QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_T));

    ac_logFile = new QAction(Ksvg::seticonSVG(":/images/svg/logfile.svg", Settings::iconColor()), i18n("Log File"), this);
    ac_logFile->setToolTip(i18n("Buffer save current tab"));
    ac_logFile->setEnabled(false);
    ac_logFile->setCheckable(true);
    ac_logFile->setChecked(false);
    ac->addAction(QStringLiteral("ac_logfile"), ac_logFile);
    ac->setDefaultShortcut(ac_logFile, QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_L));

    pbSendFile = new QAction(Ksvg::seticonSVG(":/images/svg/document-send.svg", Settings::iconColor()), i18n("Send File"), this);
	pbSendFile->setToolTip(i18n("Send file"));
    pbSendFile->setEnabled(false);
    ac->addAction(QStringLiteral("ac_sendfile"), pbSendFile);
    ac->setDefaultShortcut(pbSendFile, QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F9));

    pbClose = new QAction(Ksvg::seticonSVG(":/images/svg/window-close.svg", Settings::iconColor()), i18n("Close"), this);
	pbClose->setToolTip(i18n("Close current tab"));
    pbClose->setEnabled(false);
    ac->addAction(QStringLiteral("ac_close"), pbClose);
    ac->setDefaultShortcut(pbClose, QKeySequence(Qt::CTRL + Qt::Key_W));

    lDSR = new QLabel(this);
    lCTS = new QLabel(this);
    lDCD = new QLabel(this);
    lDSR->setPixmap(Ksvg::seticonSVG(":/images/svg/DSR.svg", "#ff007f").pixmap(24,24));
    lCTS->setPixmap(Ksvg::seticonSVG(":/images/svg/CTS.svg", "#ff007f").pixmap(24,24));
    lDCD->setPixmap(Ksvg::seticonSVG(":/images/svg/DCD.svg", "#ff007f").pixmap(24,24));
}

void KSerialport::newFile()
{
    doDevice->newFile("serialport");
}

void KSerialport::showMenu()
{
    QMenu* view_menu = part()->menu(QStringLiteral("ViewMenu"));
    if (!view_menu)
        return;
    part()->unplugActionList(QStringLiteral("view_columns_list"));
    part()->plugActionList(QStringLiteral("view_columns_list"), column_action_list);
}

void KSerialport::editItem(const QString& path)
{
    doDevice->serialportName->setText(path);
    doDevice->editProfile("serialport", listWidget->path);
}

void KSerialport::openFile(const QString& fileName)
{
    QSettings *settings = new QSettings(fileName, QSettings::IniFormat);
    settings->beginGroup("Serialport");
    serialport = new TSerialport(settings->value("Comport").toString(),
                                 settings->value("BaudRate").toInt(),
                                 settings->value("DataBits").toInt(),
                                 settings->value("Parity").toInt(),
                                 settings->value("StopBits").toInt(),
                                 settings->value("FlowControl").toInt(),
                                 settings->value("SendBreak").toInt(),
                                 this);

    bool open = serialport->openPort();
    editor = new KEditor;
    if(open) {
        tabWidget->addTab(editor, Ksvg::seticonSVG(":/images/svg/rs-232.svg", Settings::iconColor()), settings->value("Comport").toString());
        editors.append(editor);
        serialports.insert(COUNTER, serialport);
        connect(serialport, &TSerialport::sendData, editor, &KEditor::plainText);
//        connect(serialport, &TSerialport::sendHandleError, this, &KSerialport::handleError);
        connect(editor, &KEditor::sendText, serialport, &TSerialport::sendText);
        connect(editor, &KEditor::linemodeChanged, serialport, &TSerialport::setLineMode);
        connect(editor, &KEditor::changeTimer, serialport, &TSerialport::changeTimer);

        serialport->setLineMode(Settings::lineMode());
        pbRTS->setChecked(false);
        pbDTR->setChecked(false);
        timeStamp->setChecked(Settings::timeStamp());
        newLineAfter->setChecked(Settings::newLine());
        editor->timeStamp = Settings::timeStamp();
        editor->newLineAfter = Settings::newLine();
        serialport->TabId = tabWidget->currentIndex();
        tabWidget->widget(tabWidget->count()-1)->setObjectName(QString::number(COUNTER++));
    }
}

void KSerialport::openConnection()
{
    if(tabWidget->currentIndex() > 0) {
        if(currentSerialport()->status()) {
            currentSerialport()->closeDevice();
        } else {
            currentSerialport()->openPort();
        }
    }
    connectState();
}

void KSerialport::setRTS()
{
    currentSerialport()->setRTS(pbRTS->isChecked());
    if (currentSerialport()->isRTS())
        pbRTS->setIcon(Ksvg::seticonSVG(":/images/svg/RTS.svg", "#55ff7f"));
    else
        pbRTS->setIcon(Ksvg::seticonSVG(":/images/svg/RTS.svg", "#ff007f"));
}

void KSerialport::setDTR()
{
    currentSerialport()->setDTR(pbDTR->isChecked());
    if (currentSerialport()->isDTR())
        pbDTR->setIcon(Ksvg::seticonSVG(":/images/svg/DTR.svg", "#55ff7f"));
    else
        pbDTR->setIcon(Ksvg::seticonSVG(":/images/svg/DTR.svg", "#ff007f"));
}

void KSerialport::pinoutSignals(QList<bool> pSignals, QString objctNameTerminal)
{
    if(currentEditor()->objectName() == objctNameTerminal) {
        if(pSignals[8])
            lDSR->setPixmap(Ksvg::seticonSVG(":/images/svg/DSR.svg", "#55ff7f").pixmap(24,24));
        else
            lDSR->setPixmap(Ksvg::seticonSVG(":/images/svg/DSR.svg", "#ff007f").pixmap(24,24));

        if(pSignals[3])
            lCTS->setPixmap(Ksvg::seticonSVG(":/images/svg/CTS.svg", "#55ff7f").pixmap(24,24));
        else
            lCTS->setPixmap(Ksvg::seticonSVG(":/images/svg/CTS.svg", "#ff007f").pixmap(24,24));

        if(pSignals[4])
            lDCD->setPixmap(Ksvg::seticonSVG(":/images/svg/DCD.svg", "#55ff7f").pixmap(24,24));
        else
            lDCD->setPixmap(Ksvg::seticonSVG(":/images/svg/DCD.svg", "#ff007f").pixmap(24,24));
    }
}

void KSerialport::sendText(QString text, int time)
{
    if(tabWidget->currentIndex() > 0) {
        currentSerialport()->changeTimer(time);
        currentSerialport()->sendText(text);
    }
    currentEditor()->plainText(QString(text+"\n").toUtf8());
}

void KSerialport::sendFile()
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
    for (int i = 0; i < data.size(); i++) {
        if ((i % step) == 0) {
            m_progress->setValue(i / step);
            qApp->processEvents();
        }
        bool rc = currentSerialport()->sendTextTimer(data.at(i), currentSerialport()->timedelay);
        if ((data.at(i) == '\n') || (data.at(i) == '\r')) {
            // waiting twice as long after bytes whigh might by line ends
            //(this helps some uCs)
            millisleep(static_cast<unsigned int>(serialport->timedelay));
        }
        if (!rc) {
            QMessageBox::information(this, tr("Comm error"), tr("Sending failed (%1/%2").arg(i).arg(data.count()));
            break;
        }
        if (m_progress->wasCanceled())
            break;
    }
    delete m_progress;
    m_progress = nullptr;
    currentEditor()->plainText(QString("Send Success :" + filename).toUtf8());
}

void KSerialport::currentUpdate()
{
    currentEditor();
    bool checkAction = false;

    if(tabWidget->currentIndex() > 0 ) {
        disconnect(pbOpen, &QAction::triggered, this, &KSerialport::openConnection);
        disconnect(pbRTS, &QAction::triggered, this, &KSerialport::setRTS);
        disconnect(pbDTR, &QAction::triggered, this, &KSerialport::setDTR);
        disconnect(pbSendFile, &QAction::triggered, this, &KSerialport::sendFile);
        disconnect(timeStamp, &QAction::triggered, this, &KSerialport::setTimeStamp);
        disconnect(newLineAfter, &QAction::triggered, this, &KSerialport::setNewLineAfter);
        disconnect(ac_logFile, &QAction::triggered, this, &KSerialport::setLogStart);
//        disconnect(currentEditor(), &serialportGUI::pinSignals, this, &KSerialport::pinoutSignals);
        disconnect(pbClose, &QAction::triggered, this, &KSerialport::closeProfile);

        checkAction = true;

        connect(pbOpen, &QAction::triggered, this, &KSerialport::openConnection);
        connect(pbRTS, &QAction::triggered, this, &KSerialport::setRTS);
        connect(pbDTR, &QAction::triggered, this, &KSerialport::setDTR);
        connect(timeStamp, &QAction::triggered, this, &KSerialport::setTimeStamp);
        connect(newLineAfter, &QAction::triggered, this, &KSerialport::setNewLineAfter);
        connect(ac_logFile, &QAction::triggered, this, &KSerialport::setLogStart);
        connect(pbSendFile, &QAction::triggered, this, &KSerialport::sendFile);
        connect(pbClose, &QAction::triggered, this, &KSerialport::closeProfile);
//        connect(currentEditor(), &serialportGUI::pinSignals, this, &KSerialport::pinoutSignals);
    }

    pbOpen->setEnabled(checkAction);
    pbRTS->setEnabled(checkAction);
    pbDTR->setEnabled(checkAction);
    lDSR->setEnabled(checkAction);
    lCTS->setEnabled(checkAction);
    lDCD->setEnabled(checkAction);
    timeStamp->setEnabled(checkAction);
    newLineAfter->setEnabled(checkAction);
    ac_logFile->setEnabled(checkAction);
    pbSendFile->setEnabled(checkAction);
    pbClose->setEnabled(checkAction);

    connectState();
    if(tabWidget->currentIndex() > 0) {
        serialport->TabId = tabWidget->currentIndex();
        timeStamp->setChecked(currentEditor()->timeStamp);
        newLineAfter->setChecked(currentEditor()->newLineAfter);
        ac_logFile->setChecked(currentSerialport()->isLogFile);
    }
}

void KSerialport::setNewLineAfter() { currentEditor()->newLineAfter = newLineAfter->isChecked(); }
void KSerialport::setTimeStamp() { currentEditor()->timeStamp = timeStamp->isChecked(); }
void KSerialport::setLogStart() { currentSerialport()->logStart(ac_logFile->isChecked()); }

void KSerialport::connectState()
{
    if(tabWidget->currentIndex() > 0) {
        if(currentSerialport()->status()) {
            pbOpen->setIcon(Ksvg::seticonSVG(":/images/svg/plug_connect.svg", Settings::iconColor()));
            pbOpen->setText(i18n("Connected"));
        }
        else {
            pbOpen->setIcon(Ksvg::seticonSVG(":/images/svg/plug_disconnect.svg", Settings::iconColor()));
            pbOpen->setText(i18n("Disconnect"));
        }

        pbRTS->setChecked(currentSerialport()->isRTS());
        if (currentSerialport()->isRTS())
            pbRTS->setIcon(Ksvg::seticonSVG(":/images/svg/RTS.svg", "#55ff7f"));
        else
            pbRTS->setIcon(Ksvg::seticonSVG(":/images/svg/RTS.svg", "#ff007f"));

        pbDTR->setChecked(currentSerialport()->isDTR());
        if (currentSerialport()->isDTR())
            pbDTR->setIcon(Ksvg::seticonSVG(":/images/svg/DTR.svg", "#55ff7f"));
        else
            pbDTR->setIcon(Ksvg::seticonSVG(":/images/svg/DTR.svg", "#ff007f"));

        timeStamp->setChecked(currentEditor()->timeStamp);
        newLineAfter->setChecked(currentEditor()->newLineAfter);
        ac_logFile->setChecked(currentSerialport()->isLogFile);
    }
}

TSerialport* KSerialport::currentSerialport()
{
    return serialports[tabWidget->currentWidget()->objectName().toInt()];
}

KEditor* KSerialport::currentEditor()
{
    return static_cast<KEditor*>(tabWidget->currentWidget());
}

int KSerialport::tabIndex()
{
    return tabWidget->currentIndex();
}

void KSerialport::closeProfile()
{
    currentSerialport()->closeDevice();
    serialports.remove(tabWidget->currentWidget()->objectName().toInt());
    currentEditor()->closeProfile();
    editors.removeOne(currentEditor());
    currentUpdate();
}

//void KSerialport::windowclose()
//{
//    for(int i=1; i < COUNTER; i++) {
//            serialports[i]->closeDevice();
//    }
//}

void KSerialport::updateActions(){}

}
