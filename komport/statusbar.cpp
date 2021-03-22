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

#include "statusbar.h"
#include "util/ksvg.h"

#include <QLabel>
#include <QProgressBar>
#include <KLocalizedString>
#include <KStatusBarOfflineIndicator>

namespace Komport
{
StatusBar::StatusBar(QWidget* parent) : QStatusBar(parent)
{
//    status = new QLabel(i18n("DHT: off"), this);
//    status->setFrameShape(QFrame::Box);
//    status->setFrameShadow(QFrame::Sunken);
//    addPermanentWidget(status);
//    addPermanentWidget(new KStatusBarOfflineIndicator(this));

//    Ksvg ksvg;

//    QLabel *dsr = new QLabel(this);
//    QLabel *cts = new QLabel(this);
//    QLabel *dcd = new QLabel(this);
//    dsr->setPixmap(ksvg.seticonSVG(":/images/icons/svg/DSR.svg", "#ff007f").pixmap(24,24));
//    cts->setPixmap(ksvg.seticonSVG(":/images/icons/svg/CTS.svg", "#ff007f").pixmap(24,24));
//    dcd->setPixmap(ksvg.seticonSVG(":/images/icons/svg/DCD.svg", "#ff007f").pixmap(24,24));
//    addPermanentWidget(dsr);
//    addPermanentWidget(cts);
//    addPermanentWidget(dcd);
}

StatusBar::~StatusBar()
{
}

QProgressBar* StatusBar::createProgressBar()
{
    QProgressBar* pb = new QProgressBar(this);
    pb->setMaximumHeight(height());
    addPermanentWidget(pb);
    return pb;
}

void StatusBar::removeProgressBar(QProgressBar* pb)
{
    removeWidget(pb);
    pb->deleteLater();
}

void StatusBar::message(const QString& msg)
{
    showMessage(msg, 30 * 1000);
}
}
