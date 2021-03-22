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

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDBusConnection>
#include <QLockFile>
#include <QtGlobal>
#include <QDir>
#include <QMessageBox>
#include <QLoggingCategory>
#include <KLocalizedString>
#include <KAboutData>
#include <KCrash>
#include "gui.h"
#include <ktversion.h>

using namespace Komport;

int main(int argc, char** argv)
{
    /**
     * init resources from our static lib
     */
    Q_INIT_RESOURCE(komport);

    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    KLocalizedString::setApplicationDomain("komport");
    KLocalizedString::availableDomainTranslations("komport");

    /**
     * enable high dpi support
     */
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0) && defined(Q_OS_WIN)
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif

    /**
     * For Windows and macOS: use Breeze if available
     * Of all tested styles that works the best for us
     */
#if defined(Q_OS_MACOS) || defined(Q_OS_WIN)
    QApplication::setStyle(QStringLiteral("breeze"));
#endif

    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("komport"));
    app.setWindowIcon(QIcon(":/images/icons/32-apps-komport.png"));
    KCrash::initialize();
    QCommandLineParser parser;
    KAboutData about(QStringLiteral("komport"), i18nc("@title", "Komport"),
                     QStringLiteral(KOMPORT_VERSION_MACRO), i18n("komport client"),
                     KAboutLicense::GPL, i18nc("@info:credit", "(C) 2019 - 2020 Masoud Khorram"),
                     QString(),
                     QStringLiteral("https://github.com/khorram-dev/komport"));

    about.setOrganizationDomain(QByteArray("kde.org"));
    about.setDesktopFileName(QStringLiteral("org.kde.komport"));

    about.addAuthor(i18n("Masoud Khorram"), QString(), QStringLiteral("masoudkh62@gmail.com"), QStringLiteral("https://github.com/khorram-dev/komport/releases"));
    about.addCredit(i18n("HCD Research"), i18n("HCD Research is a UK based company that has specialised in the design and manufacture of oven controlled crystal oscillators (OCXO) for over five decades."), QStringLiteral("info@hcdresearch.co.uk"));
    about.setupCommandLine(&parser);
    parser.process(app);
    about.processCommandLine(&parser);
    about.setProgramLogo(QIcon(QStringLiteral(":/images/icons/64-apps-komport.png")));
    KAboutData::setApplicationData(about);

    QLockFile lockFile(QDir::temp().absoluteFilePath("komport_Lock.lock"));

    /* Trying to close the Lock File, if the attempt is unsuccessful for 100 milliseconds,
     * then there is a Lock File already created by another process.
     / Therefore, we throw a warning and close the program
     * */
    if(!lockFile.tryLock(100))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("The application is already running.\n"
                       "Allowed to run only one instance of the application.");
        msgBox.exec();
        return 1;
    }

    GUI *gui = new GUI;
    gui->show();
	
    app.exec();
    return 0;
}
