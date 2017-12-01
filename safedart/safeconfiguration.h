/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: safeconfiguration.h
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

#include <reflectable.h>
#include <settingsconfiguration.h>

/*!
 * \brief An implementation of Configuration which uses the SAFE configuration
 * file.
 *
 * Implementing a subclass of SettingsConfiguration which represents a specific
 * configuration file allows a Builder to create and manage the Configuration so
 * that it can be accessed through any code which has access to the Builder.
 *
 * This may also serve as a template for a Configuration which uses a
 * software-specific configuration file via QSettings.
 *
 * \ingroup SAFE-DART-Host
 */
class SafeConfiguration :
        public SettingsConfiguration,
        public Reflectable<SafeConfiguration>
{
    Q_OBJECT

public:
    /*!
     * \brief Gets the path to the file that will be used for the next
     * SafeConfiguration that is created.
     *
     * \return The path to the file that will be used for the next
     * SafeConfiguration that is created.
     */
    static QString getFile();

    /*!
     * \brief Sets the path to the file that will be used for the next
     * SafeConfiguration that is created.
     *
     * \param file The path to the file that should be used for the next
     * SafeConfiguration that is created.
     */
    static void setFile(const QString &file);

    /*!
     * \brief Creates a SafeConfiguration.
     *
     * \param parent The parent QObject of this QObject.
     */
    Q_INVOKABLE explicit SafeConfiguration(QObject *parent = 0);

protected:
    /*!
     * \brief The path to the file to use the next time a SafeConfiguration is
     * created.
     */
    static QString _file;

    /*!
     * \brief The instance of QSettings which is used by the base
     * SettingsConfiguration.
     */
    QSettings _safeSettings;
};
