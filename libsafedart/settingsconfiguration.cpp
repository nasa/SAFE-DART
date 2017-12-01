/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: settingsconfiguration.cpp
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

#include "settingsconfiguration.h"

// ********************************************************************** */
SettingsConfiguration::SettingsConfiguration(QSettings *settings, QObject *parent) :
    QObject(parent),
    _settings(settings)
// ********************************************************************** */
{
} // SettingsConfiguration::SettingsConfiguration(QSettings *settings, QObject *parent) :

// ********************************************************************** */
void SettingsConfiguration::clear()
// ********************************************************************** */
{
    QMutexLocker _locker(&_settingsMutex);
    Q_UNUSED(_locker);

    _settings->clear();
} // void SettingsConfiguration::clear()

// ********************************************************************** */
QVariant SettingsConfiguration::get(const QString &key, const QVariant &defaultValue)
// ********************************************************************** */
{
    QMutexLocker _locker(&_settingsMutex);
    Q_UNUSED(_locker);

    return _settings->value(key, defaultValue);
} // QVariant SettingsConfiguration::get(const QString &key, const QVariant &defaultValue)

// ********************************************************************** */
void SettingsConfiguration::remove(const QString &key)
// ********************************************************************** */
{
    QMutexLocker _locker(&_settingsMutex);
    Q_UNUSED(_locker);

    _settings->remove(key);
} // void SettingsConfiguration::remove(const QString &key)

// ********************************************************************** */
void SettingsConfiguration::set(const QString &key, const QVariant &value)
// ********************************************************************** */
{
    QMutexLocker _locker(&_settingsMutex);
    Q_UNUSED(_locker);

    return _settings->setValue(key, value);
} // void SettingsConfiguration::set(const QString &key, const QVariant &value)
