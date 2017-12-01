/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: messagetracker.cpp
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
********************************************************************** */

#include "messagetracker.h"

#include <QReadLocker>
#include <QWriteLocker>

MessageTracker *MessageTracker::_current = nullptr;

void MessageTracker::handler(QtMsgType type, const QMessageLogContext &context, const QString &content)
{
    Q_UNUSED(context);

    if(_current)
    {
        Message message;
        message.type = type;
        message.message = content;

        QWriteLocker lock(&_current->_lock);
        Q_UNUSED(lock);
        return _current->_messages.push_back(message);
    }
}

MessageTracker::MessageTracker(QObject *parent) : QObject(parent)
{
    _current = this;
    qInstallMessageHandler(&MessageTracker::handler);
}

MessageTracker::~MessageTracker()
{
    if (_current == this)
    {
        qInstallMessageHandler(nullptr);
        _current = nullptr;
    }
}

void MessageTracker::clear()
{
    QWriteLocker lock(&_lock);
    Q_UNUSED(lock);
    return _messages.clear();
}

MessageTracker::Message MessageTracker::get(int index)
{
    QReadLocker lock(&_lock);
    Q_UNUSED(lock);
    return _messages.at(index);
}

int MessageTracker::size()
{
    QReadLocker lock(&_lock);
    Q_UNUSED(lock);
    return _messages.size();
}

