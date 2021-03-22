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

#ifndef GUI_H
#define GUI_H

#include <interfaces/guiinterface.h>
//#include <interfaces/activityinterface.h>
#include <interfaces/serialportinterface.h>
#include <interfaces/socketinterface.h>
#include <interfaces/plugin.h>
#include <KSharedConfig>
#include "mainwindow.h"

class QAction;
class QTimer;
class KToggleAction;

namespace Komport
{
class PluginManager;
class PrefDialog;
class StatusBar;
class TrayIcon;
class CentralWidget;
class Settings;
class KSerialport;
class KSocket;

class GUI : public Komport::MainWindow
{
    Q_OBJECT

public:
    GUI();
    ~GUI() override;

    // Stuff implemented from GUIInterface
    Komport::MainWindow* getMainWindow() override {return this;}
    void addActivity(Activity* act) override;
    void removeActivity(Activity* act) override;
    void addPrefPage(PrefPageInterface* page) override;
    void removePrefPage(PrefPageInterface* page) override;
    void mergePluginGui(Plugin* p1) override;
    void removePluginGui(Plugin* p) override;
    StatusBarInterface* getStatusBar() override;
    void addClient(KXMLGUIClient *gClient) override;
    void setVisibleSidebar(QWidget* widget, QStringList list) override;
    void setToolView(QWidget *widget) override;
    Activity *currentActivity() override;
    QWidget *appendToolView(QWidget *widget, const QString &identifier, KMultiTabBar::KMultiTabBarPosition pos, const QIcon &icon, const QString &text) override;
    int tabIndex() override;
    QString iconColor() override;
    void statusBarChanged();
    void loadPlugins();

    PluginManager* pman;
    Komport::StatusBar* status_bar;

public Q_SLOTS:
    /// Update all actions
    void updateActions();
    void updateActionsView();

    /// Set the current activity
    void setCurrentActivity(Activity* act) override;

    /// apply gui specific settings
    void applySettings();
    void quitSave();


private Q_SLOTS:
    void showPrefDialog();
    void configureKeys();
    void configureToolbars() override;
    void newToolBarConfig();
    void activePartChanged(KParts::Part* p);
    void donation();
    void handBook();

private:
    KSerialport* m_serialport;
    KSocket* m_socket;
    CentralWidget* central;
    PrefDialog* pref_dlg;
    KParts::PartManager* part_manager;
    TrayIcon* tray_icon;
    KToggleAction* show_status_bar_action;
    KToggleAction* show_menu_bar_action;

    void setupActions();
    void loadState(KSharedConfigPtr cfg);
    void saveState(KSharedConfigPtr cfg);

protected:
    void closeEvent(QCloseEvent *event) override;

Q_SIGNALS:
    void unhandledShortcutOverride(QEvent *e);
    void updatePath(const QString&);
    void WindowIsClosing();
};
}

#endif
