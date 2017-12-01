/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: messagetracker.h
** File Date: 20160928
**
** Authors **
** Author: Jordan R. Higgins (JRH)
**
** Version and Traceability **
** Subversion: @version $Id$
**
** Revision History:
**   <Date> <Name of Change Agent>
**   Description:
**     - Bulleted list of changes.
**
** Copyright © 2017 United States Government as represented by NASA Glenn Research Center.
** No copyright is claimed in the United States under Title 17, U.S.Code. All Other Rights Reserved.
** See LICENSE.txt in the root of the repository for more details.
** 
********************************************************************** */
#pragma once

#include <QList>
#include <QObject>
#include <QReadWriteLock>

class MessageTracker :
        public QObject
{
    Q_OBJECT

public:
    struct Message
    {
        QtMsgType type;
        QString message;
    };

    explicit MessageTracker(QObject *parent = 0);
    ~MessageTracker();

    void clear();
    Message get(int index);
    int size();

protected:
    static MessageTracker *_current;
    static void handler(QtMsgType type, const QMessageLogContext &context, const QString &message);

    QReadWriteLock _lock;
    QList<Message> _messages;
};
