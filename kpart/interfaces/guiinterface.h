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

#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <QWidget>
#include <KParts/MainWindow>
#include <KMultiTabBar>
#include <KConfigSkeleton>
#include <settings.h>
#include <util/ksvg.h>
#include <libkomport_export.h>

class QWidget;
class QIcon;
class QString;
class QProgressBar;
class KMainWindow;
//class KMultiTabBar;

namespace Komport
{
class Plugin;
class PrefPageInterface;

class Activity;
class ActivityInterface;
class SerialportInterface;
class SocketInterface;
class MainInterface;
class guiTerminal;
class MainWindow;
class ToolView;
class Settings;


/**
* Base class for the status bar
* */
class LIBKOMPORT_EXPORT StatusBarInterface
{
public:
    virtual ~StatusBarInterface() {}

    /// Show a message on the statusbar for some period of time
    virtual void message(const QString& msg) = 0;


    /// Create a progress bar and put it on the right side of the statusbar
    virtual QProgressBar* createProgressBar() = 0;

    /// Remove a progress bar created with createProgressBar (pb will be deleteLater'ed)
    virtual void removeProgressBar(QProgressBar* pb) = 0;
};

/**
 * @author Masoud Khorram
 * @brief Interface to modify the GUI
 *
 * This interface allows plugins and others to modify the GUI.
*/
class LIBKOMPORT_EXPORT GUIInterface : public KParts::MainWindow
{
    Q_OBJECT

public:
    GUIInterface();
    virtual ~GUIInterface();

    /// Get a pointer to the main window
    virtual Komport::MainWindow* getMainWindow() = 0;

    /// Add an activity
    virtual void addActivity(Activity* act) = 0;

    /// Remove an activity
    virtual void removeActivity(Activity* act) = 0;

    /// Set the current activity
    virtual void setCurrentActivity(Activity* act) = 0;

    /**
    * Add a page to the preference dialog.
    * @param page The page
    */
    virtual void addPrefPage(PrefPageInterface* page) = 0;

    /**
    * Remove a page from the preference dialog.
    * @param page The page
    */
    virtual void removePrefPage(PrefPageInterface* page) = 0;

    /**
    * Merge the GUI of a plugin.
    * @param p The Plugin
    */
    virtual void mergePluginGui(Plugin* p) = 0;

    /**
    * Remove the GUI of a plugin.
    * @param p The Plugin
    */
    virtual void removePluginGui(Plugin* p) = 0;

    /// Get the status bar
    virtual StatusBarInterface* getStatusBar() = 0;

    virtual Activity* currentActivity() = 0;

    virtual int tabIndex() = 0;

    virtual QString iconColor() = 0;

    virtual void addClient(KXMLGUIClient *gClient) = 0 ;

    virtual QWidget *appendToolView(QWidget *widget, const QString &identifier, KMultiTabBar::KMultiTabBarPosition pos, const QIcon &icon, const QString &text) = 0;

    virtual void setVisibleSidebar(QWidget* widget, QStringList list) = 0;

    virtual void setToolView(QWidget *widget) = 0;

Q_SIGNALS:
    void activityChanged(Activity* act);
    void settingsChanged();

};
}

#endif
