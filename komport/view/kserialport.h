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

#ifndef KSERIALPORT_H
#define KSERIALPORT_H

#include <QWidget>
#include <QFileDialog>
#include <QScrollBar>
#include <QSerialPortInfo>
#include <QListWidget>
#include <QDesktopServices>
#include <QTabWidget>
#include <QMessageBox>
#include <QLabel>
#include <KActionCollection>
//#include <interfaces/activityinterface.h>
#include <interfaces/serialportinterface.h>

#include <KSharedConfig>
#include <KConfigGroup>

//#include "sidebar.h"
#include "newdevice.h"
#include "device/serialport.h"

namespace Komport {
class KListWidget;
class KEditor;
class KSerialport : public SerialportInterface
{
    Q_OBJECT
public:
    explicit KSerialport(QWidget* parent=nullptr);
    ~KSerialport() override;
    void setupActions(KActionCollection* ac);
    TSerialport* currentSerialport();

    QLabel *lDSR;
    QLabel *lCTS;
    QLabel *lDCD;

    void updateActions() override;
    int tabIndex();
    void reloadItems();

    void sendText(QString text, int time) override;

public Q_SLOT:
    void showMenu();
//    void handleError(const QString& error);
//    void windowclose();

private:
    QTabWidget* tabWidget;
    KListWidget *listWidget;
    KEditor* editor;
    QList<KEditor*> editors;
//    serialportGUI *serialportGui;
    TSerialport* serialport;
    QMap<int, TSerialport*> serialports;
    newDevice *doDevice;
    QWidget *wItem;
    QLabel *lName;
    QListWidgetItem *item;
    QString connectionName;

    QList<QAction*> column_action_list;
//    serialportGUI* currentguiTerminal;

    QString information;

    QAction *pbOpen;
    QAction *pbRTS;
    QAction *pbDTR;
//    QAction *pbClear;
//    QAction *pbSelectAll;
//    QAction *pbCopy;
//    QAction *pbSaveToFile;
    QAction *pbSendFile;

    QAction *newLineAfter;
//    QAction *cbRowCount;
    QAction *timeStamp;
    QAction *ac_logFile;
    QAction *pbClose;

//    KTextEditor::Editor *editor;
//    KTextEditor::View *m_view;
//    KTextEditor::Document *m_doc;

    KEditor* currentEditor();

    void newFile();
    void saveSetting();
    void loadSettings();
    void openFile(const QString &fileName);
    void openConnection();
    void setRTS();
    void setDTR();
    void pinoutSignals(QList<bool>, QString);
    void sendFile();
    void closeProfile();
    void currentUpdate();
    void connectState();
    void setTimeStamp();
    void setNewLineAfter();
    void setLogStart();
    void logFile();
    int COUNTER=0;

private slots:
//    void runProfile(int);
    void editItem(const QString &);



};
}
#endif // KSERIALPORT_H
