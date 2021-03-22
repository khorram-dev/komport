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
#ifndef KTLOGVIEWER_H
#define KTLOGVIEWER_H

#include <interfaces/activity.h>

#include <KToggleAction>
#include <KActionCollection>

#include <KActionCollection>
#include <KTextEditor/Document>
#include <KTextEditor/View>
#include <KTextEditor/Editor>

namespace Komport
{
class LogViewer : public Activity
{
    Q_OBJECT
public:
    LogViewer(QString iconColor, QWidget* parent = 0);
    ~LogViewer() override;

    void plainText(QString message);
    void openUrl(const QUrl &filePath);
    void closeProfile();
    bool newLineAfter, timeStamp;

private:
    KTextEditor::View *m_view;
    KTextEditor::Document *m_doc;
    QAction *statusBar;
    KActionCollection *ac_editor;

public Q_SLOTS:
    void editorClear();
    void editorStatusBar();

};

}

#endif
