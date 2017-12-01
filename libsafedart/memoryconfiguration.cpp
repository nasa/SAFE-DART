/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: memoryconfiguration.cpp
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

#include "memoryconfiguration.h"

// ********************************************************************** */
MemoryConfiguration::MemoryConfiguration(QObject *parent) :
    QObject(parent)
// ********************************************************************** */
{
} // MemoryConfiguration::MemoryConfiguration(QObject *parent)

// ********************************************************************** */
MemoryConfiguration::MemoryConfiguration(const QHash<QString, QVariant> &copy, QObject *parent) :
    QObject(parent),
    _hash(copy)
// ********************************************************************** */
{
} // MemoryConfiguration::MemoryConfiguration(const QHash<QString, QVariant> &copy, QObject *parent)

// ********************************************************************** */
MemoryConfiguration::MemoryConfiguration(QHash<QString, QVariant> &&move, QObject *parent) :
    QObject(parent),
    _hash(qMove(move))
// ********************************************************************** */
{
} // MemoryConfiguration::MemoryConfiguration(QHash<QString, QVariant> &&move, QObject *parent)

// ********************************************************************** */
MemoryConfiguration::MemoryConfiguration(const MemoryConfiguration &copy, QObject *parent) :
    QObject(parent)
// ********************************************************************** */
{
    QReadLocker _locker(&copy._hashLock);
    Q_UNUSED(_locker);

    _hash = copy._hash;
} // MemoryConfiguration::MemoryConfiguration(const MemoryConfiguration &copy, QObject *parent)

// ********************************************************************** */
MemoryConfiguration::MemoryConfiguration(MemoryConfiguration &&move, QObject *parent) :
    QObject(parent)
// ********************************************************************** */
{
    QWriteLocker _locker(&move._hashLock);
    Q_UNUSED(_locker);

    _hash = qMove(move._hash);
} // MemoryConfiguration::MemoryConfiguration(MemoryConfiguration &&move, QObject *parent)

// ********************************************************************** */
void MemoryConfiguration::clear()
// ********************************************************************** */
{
    QWriteLocker _locker(&_hashLock);
    Q_UNUSED(_locker);

    _hash.clear();
} // void MemoryConfiguration::clear()

// ********************************************************************** */
QVariant MemoryConfiguration::get(const QString &key, const QVariant &defaultValue)
// ********************************************************************** */
{
    QReadLocker _locker(&_hashLock);
    Q_UNUSED(_locker);

    QHash<QString, QVariant>::const_iterator iter = _hash.find(key);
    if (iter == _hash.end())
        return defaultValue;

    return iter.value();
} // QVariant MemoryConfiguration::get(const QString &key, const QVariant &defaultValue)

// ********************************************************************** */
void MemoryConfiguration::remove(const QString &key)
// ********************************************************************** */
{
    QWriteLocker _locker(&_hashLock);
    Q_UNUSED(_locker);

    _hash.remove(key);
} // void MemoryConfiguration::remove(const QString &key)

// ********************************************************************** */
void MemoryConfiguration::set(const QString &key, const QVariant &value)
// ********************************************************************** */
{
    QWriteLocker _locker(&_hashLock);
    Q_UNUSED(_locker);

    _hash[key] = value;
} // void MemoryConfiguration::set(const QString &key, const QVariant &value)

