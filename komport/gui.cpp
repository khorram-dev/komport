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

#include <QAction>
#include <QDesktopWidget>
#include <QMenuBar>
#include <QPushButton>
#include <QTimer>
#include <QUrl>
#include <QMimeDatabase>
#include <QMimeType>
#include <QDebug>

#include <KLocalizedString>
#include <KActionCollection>
#include <KConfig>
#include <KEditToolBar>
#include <KParts/PartManager>
#include <KShortcutsDialog>
#include <KStandardAction>
#include <KXMLGUIFactory>
#include <KHelpMenu>

#include <settings.h>
#include <plugin/pluginmanager.h>
#include <gui/centralwidget.h>
#include "gui.h"
#include "pref/colorschemechooser.h"
#include "pref/prefdialog.h"
#include "statusbar.h"
#include "trayicon.h"
#include "view/kserialport.h"
#include "view/ksocket.h"

namespace Komport {
GUI::GUI() : Komport::MainWindow(), pref_dlg(nullptr)
{
    setToolViewStyle(KMultiTabBar::KDEV3ICON);
    part_manager = new KParts::PartManager(this);
    connect(part_manager, &KParts::PartManager::activePartChanged, this, &GUI::activePartChanged);

    QLayout *clayout = centralWidget()->layout();
    central = new CentralWidget(nullptr);
    clayout->addWidget(central);
    connect(central, &CentralWidget::changeActivity, this, &GUI::setCurrentActivity);

    m_serialport = new KSerialport(this);
    m_serialport->setContentsMargins(0, 0, 0, 0);
    m_serialport->setupActions(m_serialport->part()->actionCollection());
    addActivity(m_serialport);

    m_socket = new KSocket(this);
    m_socket->setContentsMargins(0, 0, 0, 0);
    m_socket->setupActions(m_socket->part()->actionCollection());
    addActivity(m_socket);

    tray_icon = new TrayIcon(this);
    if (Settings::showSystemTrayIcon())
        tray_icon->setVisible(true);
    else
        tray_icon->setVisible(false);

    status_bar = new Komport::StatusBar(this);
    setStatusBar(status_bar);
    setupActions();

    pman = new PluginManager(this, m_serialport, m_socket);

    setupGUI(Default, QStringLiteral("komportappui.rc"));
    applySettings();
    loadPlugins();
    loadState(KSharedConfig::openConfig());
    updateActions();
    status_bar->addPermanentWidget(m_serialport->lDSR);
    status_bar->addPermanentWidget(m_serialport->lCTS);
    status_bar->addPermanentWidget(m_serialport->lDCD);
    statusBarChanged();
    finishRestore();

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
    if(!dir.exists())
        dir.mkdir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
//    connect(this, &GUI::WindowIsClosing, m_serialport, &KSerialport::windowclose);
}

GUI::~GUI()
{
    delete part_manager;
}

void GUI::loadPlugins()
{
    pman->loadPluginList();
}

QWidget *GUI::appendToolView(QWidget *widget, const QString &identifier, KMultiTabBar::KMultiTabBarPosition pos, const QIcon &icon, const QString &text)
{
    return Komport::MainWindow::createToolView(widget, identifier, static_cast<KMultiTabBar::KMultiTabBarPosition>(pos), icon.pixmap(QSize(16, 16)), text);
}

void GUI::addClient(KXMLGUIClient* gClient)
{
    guiFactory()->addClient(gClient);
}

void GUI::addActivity(Activity* act)
{
    unplugActionList(QStringLiteral("activities_list"));
    central->addActivity(act);
    if (act->part())
        part_manager->addPart(act->part(), false);
    plugActionList(QStringLiteral("activities_list"), central->activitySwitchingActions());
}

void GUI::removeActivity(Activity* act)
{
    unplugActionList(QStringLiteral("activities_list"));
    central->removeActivity(act);
    if (act->part())
        part_manager->removePart(act->part());
    plugActionList(QStringLiteral("activities_list"), central->activitySwitchingActions());
}

void GUI::setCurrentActivity(Activity* act)
{
    central->setCurrentActivity(act);
    part_manager->setActivePart(act ? act->part() : nullptr);
    Q_EMIT activityChanged(act);
    statusBarChanged();
}

void GUI::activePartChanged(KParts::Part* p)
{
    unplugActionList(QStringLiteral("activities_list"));
    actionCollection()->removeAction(actionCollection()->action("help_donate"));
    actionCollection()->removeAction(actionCollection()->action("help_about_kde"));
#if defined(Q_OS_WIN)
    actionCollection()->removeAction(actionCollection()->action("help_contents"));
#endif
    createGUI(p);
    plugActionList(QStringLiteral("activities_list"), central->activitySwitchingActions());
}

void GUI::addPrefPage(PrefPageInterface* page)
{
    if (!pref_dlg)
    {
        pref_dlg = new PrefDialog(this);
        connect(pref_dlg, &PrefDialog::settingsChanged, this, &GUI::applySettings);
        pref_dlg->loadState(KSharedConfig::openConfig());
    }

    pref_dlg->addPrefPage(page);
}

void GUI::removePrefPage(PrefPageInterface* page)
{
    if (pref_dlg)
        pref_dlg->removePrefPage(page);
}

void GUI::statusBarChanged()
{
    if(currentActivity()->name() == "Serialport") {
        m_serialport->lDSR->setVisible(true);
        m_serialport->lCTS->setVisible(true);
        m_serialport->lDCD->setVisible(true);
    } else {
        m_serialport->lDSR->setHidden(true);
        m_serialport->lCTS->setHidden(true);
        m_serialport->lDCD->setHidden(true);
    }
}

StatusBarInterface* GUI::getStatusBar()
{
    return status_bar;
}

void GUI::setVisibleSidebar(QWidget *widget, QStringList list)
{
    for (int i = 0; i < list.size(); ++i) {
        if(!list.contains(central->currentActivity()->name()))
            hideToolView(static_cast<ToolView*>(widget));
    }
}

void GUI::setToolView(QWidget *widget)
{
    showToolView(static_cast<ToolView*>(widget));
}

void GUI::mergePluginGui(Plugin* p)
{
    if (p->parentPart() == QStringLiteral("komport"))
    {
        guiFactory()->addClient(p);
    }
    else
    {
        QList<KParts::Part*> parts = part_manager->parts();
        foreach (KParts::Part* part, parts)
        {
            if (part->domDocument().documentElement().attribute(QStringLiteral("name")) == p->parentPart())
            {
                part->insertChildClient(p);
                break;
            }
        }
    }
}

void GUI::removePluginGui(Plugin* p)
{
    if (p->parentPart() == QStringLiteral("komport"))
    {
        guiFactory()->removeClient(p);
    }
    else
    {
        QList<KParts::Part*> parts = part_manager->parts();
        foreach (KParts::Part* part, parts)
        {
            if (part->domDocument().documentElement().attribute(QStringLiteral("name")) == p->parentPart())
            {
                part->removeChildClient(p);
                break;
            }
        }
    }
}

void GUI::showPrefDialog()
{
    disconnect(pref_dlg, &PrefDialog::settingsChanged, this, &GUI::applySettings);
    if (!pref_dlg)
        pref_dlg = new PrefDialog(this);
    connect(pref_dlg, &PrefDialog::settingsChanged, this, &GUI::applySettings);
    pref_dlg->updateWidgetsAndShow();
}

void GUI::configureKeys()
{
    KShortcutsDialog::configure(actionCollection());
}

void GUI::configureToolbars()
{
    //KF5 saveMainWindowSettings(KSharedConfig::openConfig()->group("MainWindow"));
    KEditToolBar dlg(factory());
    connect(&dlg, &KEditToolBar::newToolBarConfig, this, &GUI::newToolBarConfig);
    dlg.exec();

    // Replug action list
    unplugActionList(QStringLiteral("activities_list"));
    plugActionList(QStringLiteral("activities_list"), central->activitySwitchingActions());
}

void GUI::newToolBarConfig() // This is called when OK, Apply or Defaults is clicked
{
    applyMainWindowSettings(KSharedConfig::openConfig()->group("MainWindow"));
}

void GUI::setupActions()
{
    KActionCollection* ac = actionCollection();
    KStandardAction::quit(this, SLOT(quitSave()), ac);
    show_status_bar_action = KStandardAction::showStatusbar(statusBar(), SLOT(setVisible(bool)), ac);
    show_status_bar_action->setIcon(QIcon::fromTheme(QStringLiteral("kt-show-statusbar")));
    show_menu_bar_action = KStandardAction::showMenubar(menuBar(), SLOT(setVisible(bool)), ac);
    KStandardAction::preferences(this, SLOT(showPrefDialog()), ac);
    KStandardAction::keyBindings(this, SLOT(configureKeys()), ac);
    KStandardAction::configureToolbars(this, SLOT(configureToolbars()), ac);
    // Load themes
    actionCollection()->addAction(QStringLiteral("colorscheme_menu"), new ColorSchemeChooser(ac));

#if defined(Q_OS_WIN)
    QAction *newProfile = new QAction(QIcon::fromTheme(QStringLiteral("help-contents")), i18n("Komport Handbook"), this);
    connect(newProfile, &QAction::triggered, this, &GUI::handBook);
    ac->addAction(QStringLiteral("ac_handbook"), newProfile);
    ac->setDefaultShortcut(newProfile, QKeySequence(Qt::CTRL + Qt::Key_F1));
#endif

    QAction *ac_komport_donation = new QAction(QIcon::fromTheme("help-donate"), i18n("&Donation"));
    ac->addAction(QStringLiteral("ac_komport_donation"), ac_komport_donation);
    connect(ac_komport_donation, &QAction::triggered, this, &GUI::donation);
    QAction *ac_aboutkde = new QAction(QIcon::fromTheme("kde"), i18n("&KDE Freamwork"));
    ac->addAction(QStringLiteral("ac_aboutkde"), ac_aboutkde);
    QAction *ac_donation = new QAction(QIcon::fromTheme("help-donate"), i18n("&Donation"));
    ac->addAction(QStringLiteral("ac_donation"), ac_donation);
    KHelpMenu *mHelpMenu = new KHelpMenu( this, "about");
    connect(ac_donation, &QAction::triggered, mHelpMenu, &KHelpMenu::donate);
    connect(ac_aboutkde, &QAction::triggered, mHelpMenu, &KHelpMenu::aboutKDE);

    setStandardToolBarMenuEnabled(true);
}

void GUI::applySettings()
{
    //Apply GUI update interval
    if (Settings::showSystemTrayIcon())
        tray_icon->setVisible(true);
    else
        tray_icon->setVisible(false);

    Settings::iconColor();
    m_serialport->reloadItems();
    m_socket->reloadItems();
    Q_EMIT settingsChanged();
}

void GUI::loadState(KSharedConfigPtr cfg)
{
    setAutoSaveSettings(QStringLiteral("MainWindow"), true);
    central->loadState(cfg);
    KConfigGroup g = cfg->group("MainWindow");
    resize(g.readEntry("size", QSize(400, 400)));
    move(g.readEntry("pos", QPoint(200, 200)));
    applyMainWindowSettings(g);
    bool statusbar_hidden = g.readEntry("statusbar_hidden", false);
    status_bar->setHidden(statusbar_hidden);
    show_status_bar_action->setChecked(!statusbar_hidden);

    bool menubar_hidden = g.readEntry("menubar_hidden", false);
    menuBar()->setHidden(menubar_hidden);
    show_menu_bar_action->setChecked(!menubar_hidden);

    bool hidden_on_exit = g.readEntry("hidden_on_exit", false);
    if (Settings::showSystemTrayIcon())
    {
        if (hidden_on_exit)
        {
            qDebug() << "Starting minimized" << Qt::endl;
            hide();
        }
        else
        {
            show();
        }
    }
    else
    {
        show();
    }

    setCurrentActivity(central->currentActivity());

    KConfigGroup gsidebar = cfg->group("Sidebars");
    startRestore(new KConfig, gsidebar.name());
}

void GUI::saveState(KSharedConfigPtr cfg)
{
    KConfigGroup g = cfg->group("MainWindow");
    g.writeEntry("size", size());
    g.writeEntry("pos", pos());
    saveMainWindowSettings(g);
    g.writeEntry("statusbar_hidden", status_bar->isHidden());
    g.writeEntry("menubar_hidden", menuBar()->isHidden());
    g.writeEntry("hidden_on_exit", isHidden());
    central->saveState(cfg);
    KConfigGroup gsidebar = cfg->group("Sidebars");
    saveSession(gsidebar);

    if (pref_dlg)
        pref_dlg->saveState(cfg);
    cfg->sync();
}

void GUI::quitSave()
{
    if(m_socket)
    delete m_socket;
    if(m_serialport)
    delete m_serialport;

    saveState(KSharedConfig::openConfig());
    emit WindowIsClosing();
    qApp->quit();
}

void GUI::closeEvent(QCloseEvent *event)
{
#ifdef Q_OS_MACOS
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif
    if (Settings::showSystemTrayIcon()) {
        hide();
        event->ignore();
    } else {
        quitSave();
    }
}

void GUI::updateActions()
{
    m_serialport->updateActions();
    m_socket->updateActions();
}

void GUI::updateActionsView()
{
    m_serialport->setupActions(actionCollection());
    m_socket->setupActions(actionCollection());
}

Activity* GUI::currentActivity()
{
    return central->currentActivity();
}

int GUI::tabIndex()
{
    int tabNum = 0;
    if(central->currentActivity()->name() == "Serialport")
        tabNum = m_serialport->tabIndex();
    else if(central->currentActivity()->name() == "Socket")
        tabNum = m_socket->tabNumber();
    return tabNum;
}

QString GUI::iconColor()
{
    return Settings::iconColor();
}

void GUI::donation()
{
    QString link = "https://www.paypal.com/donate?hosted_button_id=VG6P3S4ELVXJ2";
    QDesktopServices::openUrl(QUrl(link));
}

void GUI::handBook()
{
    QString link = QApplication::applicationDirPath()+"/data/doc/HTML/en/komport/index.html";
    bool blink = QDesktopServices::openUrl(QUrl::fromLocalFile(link));
    if(!blink) {
        QMessageBox msg;
        msg.setText("The path was not found:\n"+
                    QApplication::applicationDirPath()+
                    "/data/doc/HTML/en/komport/index.html");
        msg.exec();
    }
}

}
