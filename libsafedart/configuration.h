/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: configuration.h
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
#include <QString>
#include <QVariant>

/*!
 * \brief A generic interface for a source of configuration data.
 *
 * A Configuration is effectively a mapping from a key (string) to a value (any
 * type). This data could be stored in memory, in a file, in a database, etc.
 *
 * \note All implementations of Configuration should implement their operations
 * in a thread-safe manner.
 *
 * \ingroup SAFE-DART-Framework
 */
class Configuration
{
public:
    virtual ~Configuration() = 0;

    /*!
     * \brief Removes all configuration entries in this Configuration.
     */
    virtual void clear() = 0;

    /*!
     * \brief Gets the value of a configuration entry by its key.
     *
     * \param key The key of the configuration entry to get.
     * \param defaultValue The value to return if the given key does not exist.
     *
     * \return The value stored in the Configuration for the given key, or
     * \c defaultValue if there is no data for the given key.
     */
    virtual QVariant get(const QString &key, const QVariant &defaultValue = QVariant()) = 0;

    /*!
     * \brief Removes a configuration entry by its key.
     *
     * \param key The key of the configuration entry to remove.
     */
    virtual void remove(const QString &key) = 0;

    /*!
     * \brief Sets the value of a configuration entry by its key, creating it if
     * it does not exist.
     *
     * \param key The key of the configuration entry to change.
     * \param value The value to set in the configuration entry.
     */
    virtual void set(const QString &key, const QVariant &value) = 0;
};

Q_DECLARE_INTERFACE(Configuration, "Configuration")
