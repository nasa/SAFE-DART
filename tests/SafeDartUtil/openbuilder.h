/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: openbuilder.h
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

#include <builder.h>

class OpenBuilder : public Builder
{
public:
    OpenBuilder(QObject *parent = 0);

    using Builder::Instance;
    using Builder::_configuration;
    using Builder::_instances;
    using Builder::_instancesMutex;
    using Builder::_section;
};
