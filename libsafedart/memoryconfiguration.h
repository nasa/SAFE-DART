/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: memoryconfiguration.h
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

#include <QHash>
#include <QObject>
#include <QReadWriteLock>
#include <QString>
#include <QVariant>
#include <configuration.h>

/*!
 * \brief An implementation of Configuration which stores its configuration data
 * in volatile memory.
 *
 * MemoryConfiguration is particularly useful for use in unit tests, but may be
 * useful in other scenarios as well.
 *
 * \ingroup SAFE-DART-Framework
 */
class MemoryConfiguration :
        public QObject,
        public Configuration
{
    Q_OBJECT
    Q_INTERFACES(Configuration)

public:
    /*!
     * \brief Creates an empty MemoryConfiguration.
     *
     * \param parent The parent QObject of this QObject.
     */
    explicit MemoryConfiguration(QObject *parent = 0);

    /*!
     * \brief Creates a MemoryConfiguration with all of the mappings contained
     * in a QHash.
     *
     * \param copy The QHash whose mappings to copy.
     * \param parent The parent QObject of this QObject.
     */
    explicit MemoryConfiguration(const QHash<QString, QVariant> &copy, QObject *parent = 0);

    /*!
     * \brief Creates a MemoryConfiguration with all of the mappings contained
     * in a QHash.
     *
     * \param move The QHash whose mappings to move.
     * \param parent The parent QObject of this QObject.
     *
     * \note The given QHash is no longer valid after this constructor completes.
     */
    explicit MemoryConfiguration(QHash<QString, QVariant> &&move, QObject *parent = 0);

    /*!
     * \brief Creates a MemoryConfiguration with all of the mappings contained
     * in another MemoryConfiguration.
     *
     * \param copy The MemoryConfiguration whose mappings to copy.
     * \param parent The parent QObject of this QObject.
     */
    MemoryConfiguration(const MemoryConfiguration &copy, QObject *parent = 0);

    /*!
     * \brief Creates a MemoryConfiguration with all of the mappings contained
     * in another MemoryConfiguration.
     *
     * \param move The MemoryConfiguration whose mappings to move.
     * \param parent The parent QObject of this QObject.
     *
     * \note The given MemoryConfiguration is no longer valid after this constructor completes.
     */
    MemoryConfiguration(MemoryConfiguration &&move, QObject *parent = 0);

    void clear() override;
    QVariant get(const QString &key, const QVariant &defaultValue) override;
    void remove(const QString &key) override;
    void set(const QString &key, const QVariant &value) override;

protected:
    /*!
     * \brief A shorthand type name for the QHash<QString, QVariant> used by
     * MemoryConfiguration to store configuration data.
     */
    QHash<QString, QVariant> _hash;

    /*!
     * \brief A read-write lock used to ensure that access to \c _hash is
     * synchronized.
     */
    mutable QReadWriteLock _hashLock;
};
