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

#ifndef SETKSVG_H
#define SETKSVG_H

#include <QDomElement>
#include <QSvgRenderer>
#include <QPainter>
#include <QIcon>
#include <QFile>
#include <libkomport_export.h>

namespace Komport
{
class LIBKOMPORT_EXPORT Ksvg : public QIcon
{
public:
    static Ksvg *self();
    ~Ksvg();

    static QIcon seticonSVG(QString pathSvg, QString colorName)
    {
        // open svg resource load contents to qbytearray
        QFile file(pathSvg);
        file.open(QIODevice::ReadOnly);
        QByteArray bArray = file.readAll();
        // load svg contents to xml document and edit contents
        QDomDocument doc;
        doc.setContent(bArray);
        // recurivelly change color
        if(!colorName.isNull())
        setAttributeSVG(doc.documentElement(), "path", "fill", colorName);
        // create svg renderer with edited contents
        QSvgRenderer svgRenderer(doc.toByteArray());
        // create pixmap target (could be a QImage)
        QPixmap pix(svgRenderer.defaultSize());
        pix.fill(Qt::transparent);
        // create painter to act over pixmap
        QPainter pixPainter(&pix);
        // use renderer to render over painter which paints on pixmap
        svgRenderer.render(&pixPainter);
        QIcon myicon(pix);

        return myicon;
    }

    static void setAttributeSVG(QDomElement dElement, QString tagName, QString strAttr, QString attrValue)
    {
        // if it has the tagname then overwritte desired attribute
        if (dElement.tagName().compare(tagName) == 0)
        {
            dElement.setAttribute(strAttr, attrValue);
        }
        // loop all children
        for (int i = 0; i < dElement.childNodes().count(); i++)
        {
            if (!dElement.childNodes().at(i).isElement())
                continue;

            setAttributeSVG(dElement.childNodes().at(i).toElement(), tagName, strAttr, attrValue);
        }
    }

};
}

#endif // SETKSVG_H
