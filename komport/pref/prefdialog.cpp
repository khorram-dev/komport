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

#include <KLocalizedString>
#include <KConfigDialogManager>

#include "settings.h"
#include "prefdialog.h"
#include "gui.h"
#include "interfaces/prefpageinterface.h"
#include "generalpref.h"
//#include "plugin/pluginsetting.h"

namespace Komport {
PrefDialog::PrefDialog(QWidget* parent, GUI *gui) : KConfigDialog(parent, QStringLiteral("settings"), Settings::self()), gui(gui)
{
    KConfigDialogManager::propertyMap()->insert(QStringLiteral("KUrlRequester"), QByteArrayLiteral("url"));
    setFaceType(KPageDialog::List);
    addPrefPage(new GeneralPref(this));
//    PluginSetting* pluginSetting = new PluginSetting(this);
//    addPrefPage(new PluginSetting(this));
}

PrefDialog::~PrefDialog(){}

void PrefDialog::addPrefPage(PrefPageInterface* page)
{
    PrefPageScrollArea* area = new PrefPageScrollArea(page, this);
    connect(area->page, &PrefPageInterface::updateButtons, this, &PrefDialog::updateButtons);

    KPageWidgetItem* p = addPage(area, page->config(), page->pageName(), page->pageIcon());
    area->page_widget_item = p;
    pages.append(area);
    if (!isHidden())
        page->loadSettings();
}

void PrefDialog::removePrefPage(PrefPageInterface* page)
{
    foreach (PrefPageScrollArea* area, pages)
    {
        if (area->page == page)
        {
            area->takeWidget();
            pages.removeAll(area);
            removePage(area->page_widget_item);
            break;
        }
    }
}

void PrefDialog::updateWidgetsAndShow()
{
    updateWidgets();
    show();
}

void PrefDialog::updateWidgets()
{
    foreach (PrefPageScrollArea* area, pages)
        area->page->loadSettings();
}

void PrefDialog::updateWidgetsDefault()
{
    foreach (PrefPageScrollArea* area, pages)
        area->page->loadDefaults();
}

void PrefDialog::updateSettings()
{
    foreach (PrefPageScrollArea* area, pages)
        area->page->updateSettings();
}

void PrefDialog::loadState(KSharedConfigPtr cfg)
{
    KConfigGroup g = cfg->group("PrefDialog");
    QSize s = g.readEntry("size", sizeHint());
    resize(s);
}

void PrefDialog::saveState(KSharedConfigPtr cfg)
{
    KConfigGroup g = cfg->group("PrefDialog");
    g.writeEntry("size", size());
}

bool PrefDialog::hasChanged()
{
    if (KConfigDialog::hasChanged())
        return true;

    foreach (PrefPageScrollArea* area, pages)
        if (area->page->customWidgetsChanged())
            return true;

    return false;
}

PrefPageScrollArea::PrefPageScrollArea(Komport::PrefPageInterface* page, QWidget* parent) : QScrollArea(parent), page(page), page_widget_item(nullptr)
{
    setWidget(page);
    setWidgetResizable(true);
    setFrameStyle(QFrame::NoFrame);
    viewport()->setAutoFillBackground(false);
}

PrefPageScrollArea::~PrefPageScrollArea()
{
}

}
