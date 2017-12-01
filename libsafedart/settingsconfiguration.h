/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: settingsconfiguration.h
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

#include <QMutex>
#include <QObject>
#include <QSettings>
#include <configuration.h>

/*!
 * \brief An implementation of Configuration which stores its configuration data
 * in QSettings.
 *
 * SettingsConfiguration uses QSettings, which is persistent; as a result, it is
 * useful for use in production software for storing editable configuration data.
 *
 * Custom configuration files can be shared throughout the software by extending
 * SettingsConfiguration. Create a no-arg constructor which constructs the
 * underlying SettingsConfiguration with an appropriately configured QSettings.
 * This subtype can then be managed via a Builder.
 *
 * \ingroup SAFE-DART-Framework
 */
class SettingsConfiguration :
        public QObject,
        public Configuration
{
    Q_OBJECT
    Q_INTERFACES(Configuration)

public:
    /*!
     * \brief Creates a SettingsConfiguration which uses the given QSettings.
     *
     * \param settings The QSettings object to be used by the
     * SettingsConfiguration. Must outlast the SettingsConfiguration.
     * \param parent The parent QObject for this QObject.
     *
     * \note If the SettingsConfiguration is bypassed to access the QSettings
     * directly, care should be taken that there are no synchronization
     * problems. SettingsConfiguration is synchronized internally, but external
     * access is not protected.
     */
    explicit SettingsConfiguration(QSettings *settings, QObject *parent = 0);

    void clear() override;
    QVariant get(const QString &key, const QVariant &defaultValue) override;
    void remove(const QString &key) override;
    void set(const QString &key, const QVariant &value) override;

protected:
    /*!
     * \brief A pointer to the QSettings object wrapped by this
     * SettingsConfiguration.
     */
    QSettings *_settings;

    /*!
     * \brief A mutex used to ensure that access to \c _settings is exclusive.
     */
    QMutex _settingsMutex;
};
