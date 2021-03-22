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

#ifndef COLORSCHEMECHOOSER_H
#define COLORSCHEMECHOOSER_H

#include <QAction>
#include <QApplication>
#include <QObject>
#include <QStyle>
#include <QStyleFactory>

#include <KColorSchemeManager>

class KActionCollection;

/**
 * Provides a menu that will offer to change the color scheme
 *
 * Furthermore, it will save the selection in the user configuration.
 */
class ColorSchemeChooser : public QAction
{
public:
    ColorSchemeChooser(QObject *parent);

private Q_SLOTS:
    void slotSchemeChanged(QAction *triggeredAction);

private:
    QString loadCurrentScheme() const;
    void changeStyleSystem(bool checked);
    void changeStyleCss(bool checked);
    void loadFileStyle(QString name) const;
    void changeAction(QMenu *menu, QString name) const;
    void saveCurrentScheme(int id, const QString &name);

    KColorSchemeManager* manager;
    QAction *action;
    QActionGroup *styleActionGroup;
};

#endif // COLORSCHEMECHOOSER_H
