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

#ifndef KT_PREFDIALOG_HH
#define KT_PREFDIALOG_HH

#include <QList>
#include <QScrollArea>

#include <KConfigDialog>
#include <kconfiggroup.h>


namespace Komport
{
class GUI;
class PrefPageInterface;
class PrefPageScrollArea;

/**
* Komport's preferences dialog, this uses the new KConfigDialog class which should make our live much easier.
* In order for this to work properly the widgets have to be named kcfg_FOO where FOO is a somehting out of our settings class.
*
* The use of KConfigDialog should deprecate PrefPageInterface.
* */
class PrefDialog : public KConfigDialog
{
    Q_OBJECT
public:
    PrefDialog(QWidget* parent = nullptr, GUI* gui = nullptr);
    ~PrefDialog() override;

    /**
     * Add a pref page to the dialog.
     * @param page The page
     * */
    void addPrefPage(PrefPageInterface* page);

    /**
     * Remove a pref page.
     * @param page The page
     * */
    void removePrefPage(PrefPageInterface* page);

    /**
     * Update the widgets and show
     */
    void updateWidgetsAndShow();

    /**
     * Load the state of the dialog
     */
    void loadState(KSharedConfigPtr cfg);

    /**
     * Save the state of the dialog
     */
    void saveState(KSharedConfigPtr cfg);

protected:
    void updateWidgets() override;
    void updateWidgetsDefault() override;
    void updateSettings() override;
    bool hasChanged() override;

private:
    GUI* gui;
    QList<PrefPageScrollArea*> pages;
};

class PrefPageScrollArea : public QScrollArea
{
public:
    PrefPageScrollArea(PrefPageInterface* page, QWidget* parent = nullptr);
    ~PrefPageScrollArea() override;

    PrefPageInterface* page;
    KPageWidgetItem* page_widget_item;
};
}


#endif
