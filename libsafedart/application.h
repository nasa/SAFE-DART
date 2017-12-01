/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: application.h
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
** Copyright © 2017 United States Government as represented by NASA Glenn Research Center. No copyright is claimed in the United States under Title 17, U.S.Code. All Other Rights Reserved. See LICENSE.txt in the root of the repository for more details.
**
********************************************************************** */
#pragma once

#include <QObject>

/*!
 * \brief An application which may be hosted by SAFE-DART.
 *
 * By providing an implementation of Application which may be discovered via
 * reflection (see Reflectable), a module may provide an entry point which may
 * be used by SAFE-DART to run an application.
 *
 * An implementation of Application needs to do nothing besides provide a
 * \c main method SAFE-DART can call to execute the application. It is optional
 * (but recommended) that the implementation has a constructor which takes a
 * <tt>Builder *</tt> as a parameter in order for the application to be able to
 * dynamically instantiate other types using SAFE-DART. See
 * \c Builder::get(const char *) for more information about construction.
 *
 * \ingroup SAFE-DART-Framework
 */
class Application
{
public:
    /*!
     * \brief The entry point for an application hosted by SAFE-DART.
     *
     * This function is identical to the standard \c main entry point in its
     * signature, so applications can be written in much the same way as with an
     * executable.
     *
     * \param argc The number of command-line arguments given to the
     * application by the operating system.
     * \param argv The command-line arguments given to the application, as given
     * by the operating system.
     *
     * \return The result of running the application.
     * \retval 0 The application completed successfully.
     * \retval !0 The application completed with an error.
     */
    virtual int main(int argc, char **argv) = 0;
};

Q_DECLARE_INTERFACE(Application, "Application")
