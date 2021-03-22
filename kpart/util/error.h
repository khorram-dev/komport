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
#ifndef ERROR_H
#define ERROR_H

#include <qstring.h>
#include <libkomport_export.h>

namespace Komport
{

/**
@author Masoud Khorram
*/
class LIBKOMPORT_EXPORT Error
{
    QString msg;
public:
    Error(const QString & msg);
    virtual ~Error();

    QString toString() const {return msg;}
};

class LIBKOMPORT_EXPORT Warning
{
    QString msg;
public:
    Warning(const QString & msg);
    virtual ~Warning();

    QString toString() const {return msg;}
};
}

#endif
