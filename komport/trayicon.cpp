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

#include "trayicon.h"
#include "gui.h"
#include <QIcon>
#include <QMessageBox>

namespace Komport
{

TrayIcon::TrayIcon(GUI* gui) : gui(gui)
{
    maximizeAction = new QAction(tr("Ma&ximize"), this);
    restoreAction = new QAction(tr("Mi&nimize"), this);
    quitAction = new QAction(tr("&Quit"), this);
    menu = new QMenu(gui);
    menu->addAction(maximizeAction);
    menu->addAction(restoreAction);
    menu->addSeparator();
    menu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(menu);

    connect(maximizeAction, &QAction::triggered, this, &TrayIcon::showMaximized);
    connect(restoreAction, &QAction::triggered, this, &TrayIcon::changeState);
    connect(quitAction, &QAction::triggered, gui, &GUI::quitSave);

    QIcon icon(":/images/png/64-apps-komport.png");
    trayIcon->setIcon(icon);
    gui->setWindowIcon(icon);
    trayIcon->setToolTip("Komport");

    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &TrayIcon::messageClicked);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &TrayIcon::iconActivated);
}

TrayIcon::~TrayIcon(){}

void TrayIcon::setVisible(bool visible)
{
    if(visible)
        trayIcon->show();
    else
        trayIcon->hide();
}

void TrayIcon::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        changeState();
        break;
    case QSystemTrayIcon::MiddleClick:
//        showMessage(0, "Komport", "Komport MiddleClick");
        break;
    default:
        ;
    }
}

void TrayIcon::showMessage(int value, QString title, QString message)
{
    QSystemTrayIcon::MessageIcon msgIcon = QSystemTrayIcon::MessageIcon(value);

    if (value == -1) { // custom icon
        QIcon icon(":/images/png/64-apps-komport.png");
        trayIcon->showMessage(title, message, icon, 3000);
    } else {
        trayIcon->showMessage(title, message, msgIcon, 3000);
    }
}

void TrayIcon::messageClicked()
{
    QMessageBox::information(nullptr, tr("Systray"),
                             tr("Sorry, I already gave what help I could.\n"
                                "Maybe you should try asking a human?"));
}

void TrayIcon::showMaximized()
{
    gui->showMaximized();
    restoreAction->setText(tr("Mi&nimize"));
}

void TrayIcon::changeState()
{
    if(gui->isHidden()) {
        gui->setVisible(true);
        restoreAction->setText(tr("Mi&nimize"));
    } else {
        gui->setHidden(true);
        restoreAction->setText(tr("&Show"));
    }
}


}
