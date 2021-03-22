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
#ifndef WAITJOB_H
#define WAITJOB_H

#include <kio/job.h>
#include <qlist.h>
#include <libkomport_export.h>

namespace Komport
{

/**
* @author Masoud Khorram
*
* Job to wait for a certain amount of time or until one or more QObject's have
* finished.
*/
class LIBKOMPORT_EXPORT WaitJob : public KIO::Job
{
    Q_OBJECT

public:
    WaitJob(qint32 millis);
    ~WaitJob() override;

    virtual void kill(bool quietly=true);

    /**
    * Add an QObject;
    * @param op The operation
    */
    void addQObject(QObject* op);

    /**
    * Add a KIO::Job to wait on;
    * @param job The job
    */
    void addQObject(KIO::Job* job);


    /**
    * Execute a WaitJob
    * @param job The Job
    */
    static void execute(WaitJob* job);

    /// Are there any QObject's we need to wait for
    bool needToWait() const {return exit_ops.count() > 0;}

private Q_SLOTS:
    void timerDone();
    void operationFinished(QObject* op);

private:
    QList<QObject*> exit_ops;
};

LIBKOMPORT_EXPORT void SynchronousWait(qint32 millis);

}

#endif
