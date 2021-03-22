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

#include "colorschemechooser.h"

#include <QActionGroup>
#include <QDirIterator>
#include <QFileInfo>
#include <QMenu>
#include <QModelIndex>
#include <QStandardPaths>
#include <QStringList>
#include <QtGlobal>
#include <QDebug>

#include <KActionCollection>
#include <KActionMenu>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <kconfigwidgets_version.h>

ColorSchemeChooser::ColorSchemeChooser(QObject *parent)
    : QAction(parent)
{
    QMenu *menuscm = new QMenu;
    menuscm->setIcon(QIcon::fromTheme(QStringLiteral("preferences-desktop-color")));

    manager = new KColorSchemeManager(parent);
    auto selectionMenu = manager->createSchemeSelectionMenu(i18n("&System Scheme"), this);
    connect(selectionMenu->menu(), &QMenu::triggered, this, &ColorSchemeChooser::slotSchemeChanged);
    manager->activateScheme(manager->indexForScheme(""));

    KActionMenu* actionsystem = new KActionMenu(QIcon::fromTheme(QStringLiteral("financial-categories")), i18n("Custom Style"), parent);
    styleActionGroup = new QActionGroup(this);
    const QStringList styleKeys = QStyleFactory::keys();
    for (const QString &styleName : styleKeys) {
        action = new QAction(styleName, styleActionGroup);
        action->setData(styleName);
        action->setCheckable(true);
        connect(action, &QAction::triggered, this, &ColorSchemeChooser::changeStyleSystem);
        actionsystem->addAction(action);
    }

    QDir dir(QApplication::applicationDirPath()+"/colorscheme");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        action = new QAction(fileInfo.fileName(), styleActionGroup);
        action->setData(fileInfo.fileName());
        action->setCheckable(true);
        connect(action, &QAction::triggered, this, &ColorSchemeChooser::changeStyleCss);
        actionsystem->addAction(action);
    }

    setMenu(menuscm);
    menuscm->addAction(selectionMenu);
    menuscm->addMenu(actionsystem->menu());
    menu()->setIcon(QIcon::fromTheme(QStringLiteral("preferences-desktop-color")));
    menu()->setTitle(i18n("&Color Scheme"));

    loadCurrentScheme();
}

void ColorSchemeChooser::loadFileStyle(QString name) const
{
    QFile styleFile(QApplication::applicationDirPath()+"/colorscheme/"+name);
    styleFile.open( QFile::ReadOnly );
    // Apply the loaded stylesheet
    QString style = styleFile.readAll();
    qApp->setStyleSheet(style);
    styleFile.close();
}

void ColorSchemeChooser::changeAction(QMenu *menu, QString name) const
{
    foreach (QAction *action, menu->actions()) {
        if (action->isSeparator()) {
            qDebug("this action is a separator");
        } else if (action->menu()) {
            changeAction(action->menu(), name);
        } else {
            if (action->text() == name)
                action->setChecked(true);
            else
                action->setChecked(false);
        }
    }
}

void ColorSchemeChooser::slotSchemeChanged(QAction *triggeredAction)
{
    QString name = KLocalizedString::removeAcceleratorMarker(triggeredAction->text());
    qApp->setStyleSheet("");
    changeAction(menu(), triggeredAction->text());
    saveCurrentScheme(0, name);
    manager->activateScheme(manager->indexForScheme(name));
}

void ColorSchemeChooser::changeStyleSystem(bool checked)
{
    qApp->setStyleSheet("");
    if (!checked)
        return;

    const QAction *action = qobject_cast<QAction *>(sender());
    QStyle *style = QStyleFactory::create(action->data().toString());
    Q_ASSERT(style);
    QApplication::setStyle(style);
    QApplication::setPalette(QApplication::style()->standardPalette());
    saveCurrentScheme(1, action->data().toString());
}

void ColorSchemeChooser::changeStyleCss(bool checked)
{
    qApp->setStyleSheet("");
    const QAction *action = qobject_cast<QAction *>(sender());
    loadFileStyle(action->data().toString());
    saveCurrentScheme(2, action->data().toString());
}

QString ColorSchemeChooser::loadCurrentScheme() const
{
    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup cg(config, "UiSettings");
    int id = cg.readEntry("ColorSchemeId").toInt();
    QString name = cg.readEntry("ColorSchemeName");
    switch(id) {
    case 0:
        manager->activateScheme(manager->indexForScheme(name));
        break;
    case 1:
        QApplication::setStyle(name);
        break;
    case 2:
        loadFileStyle(name);
        break;
    default:break;
    }
    changeAction(menu(), name);
    return name;
}

void ColorSchemeChooser::saveCurrentScheme(int id, const QString &name)
{
    KSharedConfigPtr config = KSharedConfig::openConfig();
    KConfigGroup cg(config, "UiSettings");
    cg.writeEntry("ColorSchemeId", id);
    cg.writeEntry("ColorSchemeName", name);
    cg.sync();
}
