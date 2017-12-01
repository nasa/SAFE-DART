/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: mockapplication.h
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
#pragma once

#include <QList>
#include <QObject>
#include <application.h>

class MockApplication :
        public QObject,
        public Application
{
    Q_OBJECT
    Q_INTERFACES(Application)

public:
    struct Call
    {
        int argc;
        char **argv;
    };

    MockApplication();

    int main(int argc, char **argv) override;

    QList<Call> _calls;
    int _result;
};
