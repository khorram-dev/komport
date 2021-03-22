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
#include "waitjob.h"
#include <QTimer>
#include <QDebug>
//#include <komport/globals.h>
//#include "log.h"

namespace Komport
{

WaitJob::WaitJob(qint32 millis) : KIO::Job(/*false*/)
{
    QTimer::singleShot(millis,this,SLOT(timerDone()));
}


WaitJob::~WaitJob()
{
    foreach (QObject* op,exit_ops)
        delete op;
}

void WaitJob::kill(bool)
{
    emitResult();
}

void WaitJob::timerDone()
{
    emitResult();
}

void WaitJob::addQObject(QObject* op)
{
    exit_ops.append(op);
    connect(op,SIGNAL(operationFinished(QObject*)),
            this,SLOT(operationFinished(QObject*)));
}

void WaitJob::addQObject(KIO::Job* job)
{
    addQObject(new QObject(job));
}


void WaitJob::operationFinished(QObject* op)
{
    if (exit_ops.count() > 0)
    {
        exit_ops.removeAll(op);
//        if (op->deleteAllowed())
//            op->deleteLater();

        if (exit_ops.count() == 0)
            timerDone();
    }
}

void WaitJob::execute(WaitJob* job)
{
    job->exec();
}

void SynchronousWait(qint32 millis)
{
    qDebug() << "SynchronousWait" << Qt::endl;
    WaitJob* j = new WaitJob(millis);
    j->exec();
}

}

