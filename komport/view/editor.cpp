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

#include <QWidget>
#include <QSettings>
#include <QScrollBar>
#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>
#include <QToolBar>
#include <QVariant>
#include <QIcon>
#include <QApplication>
#include <QComboBox>
#include <QDialog>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QAction>
#include <QSpacerItem>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QWidgetAction>
#include <QToolButton>

#include <KLocalizedString>

#include "editor.h"
#include "settings.h"
#include "util/ksvg.h"

namespace Komport {

KEditor::KEditor(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(0,0,0,0);
    verticalLayout->setSpacing(0);
    verticalLayout->setMargin(0);
    QToolBar *toolBar = new QToolBar(this);
    toolBar->setContentsMargins(0,0,0,0);
    toolBar->setMovable(true);
    toolBar->setIconSize(QSize(21, 21));
    verticalLayout->addWidget(toolBar);
    KTextEditor::Editor *editor = KTextEditor::Editor::instance();
    if (!editor) {
        QMessageBox::critical(this, i18n("KDE TextEditor"),
                                    i18n("A KDE text-editor component could not be found;\n"
                                      "please check your KDE installation."));
        qApp->exit(1);
    }
    m_doc = editor->createDocument(this);
    m_view = qobject_cast<KTextEditor::View*>(m_doc->createView(this));
    m_doc->setReadWrite(false);
    verticalLayout->addWidget(m_view);
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    QPushButton* pbSend = new QPushButton(this);
    pbSend->setObjectName(QString::fromUtf8("pbSend"));
    pbSend->setToolTip(i18n("Send"));
    pbSend->setText(i18n("Send"));
    pbSend->setEnabled(true);
    pbSend->setIcon(Ksvg::seticonSVG(":/images/svg/key-enter.svg", Settings::iconColor()));
    horizontalLayout->addWidget(pbSend);
    leSend = new QLineEdit(this);
    leSend->setObjectName(QString::fromUtf8("leSend"));
    leSend->setEnabled(true);
    horizontalLayout->addWidget(leSend);
    QComboBox* lineMode = new QComboBox(this);
    lineMode->addItem(QString());
    lineMode->addItem(QString());
    lineMode->addItem(QString());
    lineMode->addItem(QString());
    lineMode->addItem(QString());
    lineMode->setItemText(0, i18n("NONE"));
    lineMode->setItemText(1, i18n("LF"));
    lineMode->setItemText(2, i18n("CR"));
    lineMode->setItemText(3, i18n("CRLF"));
    lineMode->setItemText(4, i18n("HEX"));
    lineMode->setObjectName(QString::fromUtf8("lineMode"));
    lineMode->setCurrentIndex(Settings::lineMode());
    horizontalLayout->addWidget(lineMode);
    QSpinBox* spTimer = new QSpinBox(this);
    spTimer->setObjectName(QString::fromUtf8("spTimer"));
    spTimer->setMaximum(999999);
    spTimer->setValue(1000);
    horizontalLayout->addWidget(spTimer);
    verticalLayout->addLayout(horizontalLayout);

    ac_editor = m_view->actionCollection();
    QAction *pbClear = new QAction(Ksvg::seticonSVG(":/images/svg/document-clear.svg", Settings::iconColor()), i18nc("@action Clear terminal current", "Clear"), this);
    ac_editor->addAction(QStringLiteral("ac_clear"), pbClear);
    ac_editor->setDefaultShortcut(pbClear, QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_D));
    connect(pbClear, &QAction::triggered, this, &KEditor::editorClear);
    ac_editor->action(45)->setIcon(Ksvg::seticonSVG(":/images/svg/read-only.svg", Settings::iconColor()));
    ac_editor->action(81)->setIcon(Ksvg::seticonSVG(":/images/svg/eye.svg", Settings::iconColor()));
    statusBar = new QAction(Ksvg::seticonSVG(":/images/svg/statusbar.svg", Settings::iconColor()), i18nc("@action StatusBar", "StatusBar"), this);
    statusBar->setCheckable(true);
    statusBar->setChecked(true);
    ac_editor->addAction(QStringLiteral("ac_statusbar"), statusBar);
    ac_editor->setDefaultShortcut(statusBar, QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_D));
    connect(statusBar, &QAction::triggered, this, &KEditor::editorStatusBar);

//    for(int i=0; i < 162; i++) {
//        qDebug() << "eventFilter: KeyRelease "  <<
//                    i << " : " <<  ac_editor->action(i)->objectName() <<
//                    " : " <<  ac_editor->action(i)->text() << "‌ :‌ " <<
//                    ac_editor->action(i)->shortcut().toString() << " : " << Qt::endl;
//    }

    ac_editor->action("view_line_numbers")->setIcon(QIcon::fromTheme(QStringLiteral("text_line_spacing")));
    ac_editor->action("view_scrollbar_minimap")->setIcon(QIcon::fromTheme(QStringLiteral("flag-blue")));
    ac_editor->action("view_schemas")->setIcon(QIcon::fromTheme(QStringLiteral("colormanagement")));
    QList<QAction *> actions;
    actions <<
               pbClear <<
               ac_editor->action("edit_copy") << // Copy
               ac_editor->action("file_save_as") << // Copy
               ac_editor->action("edit_select_all") << // Select All
               ac_editor->action("view_scrollbar_minimap") << // Show Scrollbar Mini-Map
               ac_editor->action("view_line_numbers") << // Show view line numbers
               statusBar <<
               ac_editor->action("view_schemas") << // Color Scheme
               ac_editor->action("set_confdlg") //<< // Configure
               ;
    toolBar->addActions(actions);
    QMetaObject::connectSlotsByName(this);

    connect(pbSend, &QPushButton::clicked, this, &KEditor::sendClick);
    connect(leSend, &QLineEdit::returnPressed, this, &KEditor::sendClick);
    connect(lineMode, qOverload<int>(&QComboBox::currentIndexChanged), [=](int x){emit linemodeChanged(x);});
    connect(spTimer, qOverload<int>(&QSpinBox::valueChanged), [=](int x){emit changeTimer(x);});

}

KEditor::~KEditor(){}

void KEditor::editorClear()
{
    m_doc->setReadWrite(true);
    m_doc->clear();
    m_doc->setReadWrite(false);
}

void KEditor::editorStatusBar()
{
    m_view->setStatusBarEnabled(statusBar->isChecked());
}

void KEditor::sendClick()
{
    plainText(QString(leSend->text()+"\n").toUtf8());
    emit sendText(leSend->text());
    leSend->clear();
}

void KEditor::plainText(QByteArray data)
{
    QString message = QString(data);
    if (timeStamp)
        message = "[" + QDateTime::currentDateTime().toString("hh:mm:ss") + "] "+ message;
    if (newLineAfter)
        message.append("\n");

    m_doc->setReadWrite(true);
    ac_editor->action("end_of_document")->activate(QAction::ActionEvent::Trigger);
    m_doc->insertText(m_view->cursorPosition(), message);
    m_doc->setReadWrite(false);
}

void KEditor::closeProfile()
{
    delete this;
}
}
