/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: safeapplication.h
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
#include <application.h>
#include <builder.h>

/*!
 * \brief An implementation of Application which implements the functionality of
 * the SAFE host executable.
 *
 * SafeApplication implements the functionality of the SAFE host executable. It
 * primarily does three things:
 *
 * \li Load SAFE configuration
 * \li Load modules (from ./mod)
 * \li Execute configured Application
 *
 * This approach allows for the SAFE executable to host an Application from a
 * module, optionally passing the Builder as a constructor parameter to allow
 * access to objects provided by other modules. This means that software code
 * may be heavily compartmentalized into different modules, allowing for code
 * to be extensible and flexible.
 *
 * This may also serve as a template for an Application which is designed to be
 * hosted within SAFE. Note that in that case, the implementation must extend
 * Reflectable and its constructor must be Q_INVOKABLE.
 *
 * \ingroup SAFE-DART-Host
 */
class SafeApplication :
        public QObject,
        public Application
{
    Q_OBJECT
    Q_INTERFACES(Application)

public:
    /*!
     * \brief Creates a SafeApplication with the specified Builder.
     *
     * \param builder The Builder which is hosting the SafeApplication.
     * \param parent The parent QObject of this QObject.
     */
    explicit SafeApplication(Builder *builder, QObject *parent = 0);

    int main(int argc, char **argv) override;

protected slots:
    void loadedModule(QString name, QString description, QString version);

protected:
    /*!
     * \brief A pointer to the Builder which created this SafeApplication.
     */
    Builder *_builder;
};
