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

#ifndef KEDITOR_H
#define KEDITOR_H

#include <KActionCollection>
#include <KTextEditor/Document>
#include <KTextEditor/View>
#include <KTextEditor/Editor>

class QLineEdit;
namespace Komport {
class KEditor : public QWidget
{
    Q_OBJECT
public:
    explicit KEditor(QWidget* parent=nullptr);
    ~KEditor();
    void plainText(QByteArray data);
    void closeProfile();
    bool newLineAfter, timeStamp;

private:
    QLineEdit *leSend;
    KTextEditor::View *m_view;
    KTextEditor::Document *m_doc;
    QAction *statusBar;
    KActionCollection *ac_editor;

private Q_SLOTS:
    void editorClear();
    void editorStatusBar();

public Q_SLOT:
    void sendClick();

Q_SIGNALS:
    void sendText(const QString&);
    void linemodeChanged(int);
    void changeTimer(int);

};
}
#endif // KEDITOR_H
