/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: builder.cpp
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
**   6 Oct 2016 Jordan Higgins
**   Description:
**     - Fixed a deadlock caused by getting an object from a Builder from the
**       constructor of an object being created by that Builder (see FBCESD-111)
**     - Added comments to Builder::get(const char *)
**
**   17 Nov 2016 Jordan Higgins
**   Description:
**     - Added createdObject(QSharedPointer<QObject>) and
**       destroyingObject(QObject *) signals, to allow extension of the
**       instantiation process.
**
** Copyright © 2017 United States Government as represented by NASA Glenn Research Center.
** No copyright is claimed in the United States under Title 17, U.S.Code. All Other Rights Reserved.
** See LICENSE.txt in the root of the repository for more details.
**
********************************************************************** */

#include "builder.h"

// ********************************************************************** */
Builder::Builder(QObject *parent) :
    QObject(parent)
// ********************************************************************** */
{
    qRegisterMetaType<QSharedPointer<QObject>>();

} // Builder::Builder(QObject *parent)

// ********************************************************************** */
Builder::~Builder()
// ********************************************************************** */
{
} // Builder::~Builder()

// ********************************************************************** */
QSharedPointer<Configuration> Builder::configuration()
// ********************************************************************** */
{
    return _configuration;
} // QSharedPointer<Configuration> Builder::configuration()

// ********************************************************************** */
QSharedPointer<QObject> Builder::get(const char *name)
// ********************************************************************** */
{
    QByteArray objectName;

    // If a Configuration is set, use it to map the name
    if (_configuration)
    {
        QByteArray key = _section.toUtf8() + "/" + name;
        objectName = _configuration->get(key).toByteArray();
    }
    // If no Configuration was used or the key did not exist, use the given name
    // as-is
    if (objectName.isNull())
        objectName = name;

    // Get the Instance object for the object with the correct name, creating it
    // if it doesn't exist
    QMutexLocker instancesLock(&_instancesMutex);
    Instance &instance = _instances[objectName];
    instancesLock.unlock();
    // Get the existing reference to that object, return it if there is one
    QSharedPointer<QObject> result = instance._reference;
    if(result){
        return result;

    }
    // No object exists--lock the reference to ensure that only one thread is
    // creating an instance at a time
    QMutexLocker referenceLock(&instance._referenceMutex);
    Q_UNUSED(referenceLock);

    // Another thread could have been in the process of creating an instance;
    // check that there's still no instance
    result = instance._reference;
    if(result)
        return result;
    // Look up the QMetaObject for the object name; throw an exception if none
    // is found
    int metaType = QMetaType::type(objectName + '*');
    const QMetaObject *metaObject = QMetaType::metaObjectForType(metaType);
    if (!metaObject)
    {
        QString message = QString("Could not find %1 for use as %2.")
                .arg(QString(objectName))
                .arg(name);
        throw BuilderException(message);
    }

    // Create an instance of the object; try T(Builder *) first, and then T() if
    // that fails. Throw an exception if neither constructor works.
    QObject *object = metaObject->newInstance(Q_ARG(Builder *, this));
    if (!object)
    {
        object = metaObject->newInstance();
        if (!object)
        {
            QString message = QString("Failed to create %1 for use as %2.")
                    .arg(QString(objectName))
                    .arg(name);
            throw BuilderException(message);
        }
    }

    // Wrap the created object in a QSharedPointer that emits destroyingObject
    // prior to deleting the object
    result = QSharedPointer<QObject>(object, [this](QObject *object)
    {
        emit destroyingObject(object);
        delete object;
    });

    // Store a reference to the object in the Instance
    instance._reference = result;

    // Emit the created() signal for the newly-created object
    emit createdObject(result);

    // Return the created object
    return result;
} // QSharedPointer<QObject> Builder::get(const char *name)

// ********************************************************************** */
void Builder::provide(const char *name, QSharedPointer<QObject> object)
// ********************************************************************** */
{
    QByteArray objectName = name;

    // Get the Instance object for the object with the correct name, creating it
    // if it doesn't exist
    QMutexLocker instancesLock(&_instancesMutex);
    Instance &instance = _instances[objectName];
    instancesLock.unlock();

    // Set the reference in the Instance
    QMutexLocker referenceLock(&instance._referenceMutex);
    Q_UNUSED(referenceLock);
    instance._reference = object;
} // void Builder::provide(const char *name, QSharedPointer<QObject> object)

// ********************************************************************** */
QString Builder::section()
// ********************************************************************** */
{
    return _section;
} // QString Builder::section()

// ********************************************************************** */
void Builder::setConfiguration(QSharedPointer<Configuration> configuration, const QString &section)
// ********************************************************************** */
{
    _configuration = configuration;
    _section = section;
} // void Builder::setConfiguration(QSharedPointer<Configuration> configuration, const QString &section)

// ********************************************************************** */
Builder::Instance::Instance(const Instance &copy) :
  _reference(copy._reference)
// ********************************************************************** */
{
} // Builder::Instance::Instance(const Instance &copy)
