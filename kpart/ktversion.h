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
#ifndef KTVERSION_HH
#define KTVERSION_HH

#include <QtGlobal>

#define KOMPORT_VERSION_MACRO "0.0.1"

namespace Komport
{
enum VersionType
{
    ALPHA,
    BETA,
    RELEASE_CANDIDATE,
    DEVEL,
    NORMAL
};
const quint32 MAJOR = 1;
const quint32 MINOR = 0;
const quint32 RELEASE = 0;
const VersionType VERSION_TYPE = Komport::DEVEL;
const char VERSION_STRING[] = KOMPORT_VERSION_MACRO;
}

#endif
