/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: builder.h
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
#pragma once

#include <QByteArray>
#include <QException>
#include <QHash>
#include <QMutex>
#include <QObject>
#include <QSharedPointer>

#include <configuration.h>

/*!
 * \brief An exception thrown when creation of a class fails.
 *
 * \ingroup SAFE-DART-Framework
 */
class BuilderException : public QException
{
public:
    /*!
     * \brief Creates an exception from a null-terminated C string.
     *
     * \param message A message describing what caused the exception.
     */
    BuilderException(const char *message) : _message(message) {}

    /*!
     * \brief Creates an exception from a byte array.
     *
     * \param message A message describing what caused the exception.
     */
    BuilderException(const QByteArray &message) : _message(message) {}

    /*!
     * \brief Creates an exception from a QString.
     *
     * \param message A message describing what caused the exception.
     */
    BuilderException(const QString &message) : _message(message.toUtf8()) {}

    /*!
     * \brief Creates a copy of another BuilderException.
     *
     * \param copy The BuilderException whose message to copy.
     */
    BuilderException(const BuilderException &copy) : _message(copy._message) {}
    ~BuilderException() throw() {}

    /*!
     * \brief Creates a copy of this BuilderException.
     *
     * \return A pointer to a BuilderException with the same message as this.
     */
    BuilderException *clone() const { return new BuilderException(*this); }

    /*!
     * \brief Throws this BuilderException.
     */
    void raise() const { throw *this; }

    /*!
     * \brief Gets a description of what caused this BuilderException.
     *
     * \return A raw string containing a description of the cause of this
     * BuilderException.
     */
    const char *what() const throw() { return _message.data(); }

protected:
    /*!
     * \brief A byte array containing a string describing the cause of the exception.
     */
    const QByteArray _message;
};

/*!
 * \brief Builds and caches objects created through reflection.
 *
 * Builder is the foundation of the SAFE-DART architecture. It provides an
 * interface through which software can instantiate objects using reflection.
 * Instantiating objects this way allows the implementation types to be changed
 * without needing to recompile the software.
 *
 * Builder uses a configuration file which specifies which types to instantiate.
 * This allows the objects that are instantiated to be configured at run-time
 * without any additional effort required by the caller, allowing the
 * application to be reconfigured at run-time with minimal effort. With the use
 * of shared libraries, this can also allow functionality to be added or changed
 * without the need to recompile the entire software.
 *
 * Builder also keeps track of the instances it creates, reusing them when the
 * type is requested again. When an instance of Builder is used by multiple
 * libraries, this also means that any objects instantiated by the Builder may
 * be shared between those libraries.
 *
 * \ingroup SAFE-DART-Framework
 */
class Builder : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief Creates a Builder.
     *
     * \param parent The parent QObject of this Builder.
     */
    explicit Builder(QObject *parent = 0);
    ~Builder();

    /*!
     * \brief Gets an instance of a generic object by name.
     *
     * The given name is looked up in the Builder's Configuration, if any, in
     * the \c safedart section. If a match is found, its value is used as the
     * name of the type to instantiate; otherwise, the given name is used.
     * Configuration file mappings are not recursive. For instance, if the
     * configuration file has Foo=Bar and Bar=Baz, get("Foo") will attempt to
     * create an instance of the class named Bar.
     *
     * If the given type has been instantiated before, the existing object will
     * be returned if it still exists. This is true <i>even if the given name is
     * different</i>; for instance, if the configuration file has Foo=Baz and
     * Bar=Baz, get("Foo") and get("Bar") may return the same object.
     *
     * Once found, the QObject in question will be instantiated; T(Builder *) is
     * used if it is available, else T() is used.
     *
     * \param name The name of the type to instantiate.
     *
     * \return An instance of the object type associated with the given name.
     *
     * \throw BuilderException No QObject could be found with the given name.
     * \throw BuilderException The QObject with the given name could not be
     * created.
     *
     * \note The constructor used to instantiate the QObject must be marked
     * Q_INVOKABLE, or it will not be usable via reflection.
     */
    virtual QSharedPointer<QObject> get(const char *name);

    /*!
     * \brief Gets an instance of a specific type by name.
     *
     * Functions very similarly to get(const char *), but additionally casts the
     * object (safely) to the given type.
     *
     * \see get(const char *)
     * \throw LocatorException The object could not be casted to type T.
     */
    template<typename T>
    QSharedPointer<T> get(const char *name);

    /*!
     * \brief Gets an instance of a specific type.
     *
     * Functions very similarly to get<T>(const char *), but uses the name of
     * the interface T.
     *
     * \see get<T>(const char *)
     *
     * \note T must have been declared as an interface using Q_DECLARE_INTERFACE
     * for this to work correctly.
     */
    template<typename T>
    QSharedPointer<T> get();

    /*!
     * \brief Provides an instance of QObject to be associated with the given
     * name.
     *
     * This allows a custom object to be provided, which will be returned when
     * the object with the given name is requested.
     *
     * \param name The name to associate the given object with.
     * \param object The object to associate with the given name.
     *
     * \note Builder holds only a weak pointer to \c object; it must be kept
     * alive by the caller, or it will be freed and default rules will be used
     * to instantiate the object when requested.
     */
    virtual void provide(const char *name, QSharedPointer<QObject> object);

    /*!
     * \brief Gets the Configuration used by this Builder.
     *
     * \return A pointer to the Configuration used by this Builder. May be null
     * if none was set.
     */
    QSharedPointer<Configuration> configuration();

    /*!
     * \brief Gets the configuration section used by this Builder.
     *
     * \return The configuration section used by this builder.
     */
    QString section();

    /*!
     * \brief Sets the Configuration used by this Builder.
     *
     * \param configuration The Configuration to be used by this Builder.
     * \param section The section within the configuration file to use.
     */
    void setConfiguration(QSharedPointer<Configuration> configuration, const QString &section = "safedart");

signals:
    /*!
     * \brief Emitted when an object is created by the Builder.
     *
     * This signal is provided so that the application may perform additional
     * logic for newly-created objects. This signal is emitted after the object
     * is instantiated, but before get() returns the object to the caller.
     *
     * \param object A pointer to the object that was created.
     */
    void createdObject(QSharedPointer<QObject> object);

    /*!
     * \brief Emitted when an object is about to be destroyed by the Builder.
     *
     * This signal is provided so that the application may perform additional
     * logic for an object immediately before it is destroyed. This signal is
     * emitted immediately before the object is deleted, so the pointer that is
     * passed in should not be kept.
     *
     * \param object A pointer to the object that will be destroyed.
     *
     * \warning If this signal is connected in a queued non-blocking fashion,
     * the object pointer may be invalid by the time the slot is executed. Be
     * aware that <b>this can happen if automatic connection is used</b>.
     */
    void destroyingObject(QObject *object);

protected:
    /*!
     * \brief A previously-created instance of an object.
     *
     * The Instance struct contains the necessary information to maintain a
     * reference to a shared object while it exists, without preventing it from
     * being deleted. It also allows creation of the object to be synchronized
     * to ensure that only one instance of the object exists at a time.
     *
     * \note QWeakPointer itself is thread-safe, so exclusive access is not
     * required to be able to access an existing instance of the object. If
     * there is no existing instance of the object, however, only one should be
     * created. In this case, a lock is used, so that only one thread will
     * attempt to create an object. This ensures that only one instance of the
     * object will exist at any given time.
     */
    struct Instance
    {
        /*!
         * \brief A mutex which synchronizes the creation of the instance stored
         * in \c _reference.
         *
         * No lock is needed be used when reading the value of the pointer, but
         * a lock should be held when creating the object. To ensure proper
         * synchronization, \c _reference should be checked again after locking
         * the mutex and before creating an instance of the object.
         */
        QMutex _referenceMutex;

        /*!
         * \brief A weak pointer to the existing instance of the object, if any.
         */
        QWeakPointer<QObject> _reference;

        /*!
         * \brief Creates an Instance containing no value.
         */
        Instance() = default;

        /*!
         * \brief Creates a copy of the given Instance.
         *
         * \param copy The Instance whose reference to copy.
         */
        Instance(const Instance &copy);
    };

    /*!
     * \brief A pointer to the Configuration used by this Builder.
     */
    QSharedPointer<Configuration> _configuration;

    /*!
     * \brief A mapping of object name to the existing instance (if any).
     */
    QHash<QByteArray, Instance> _instances;

    /*!
     * \brief A mutex used to ensure that access to \c _instances is exclusive.
     *
     * Because it is not known whether an object exists in the map prior to
     * looking it up, an exclusive lock is used. This allows the index operator
     * to be used, which will add the key if it does not already exist in a
     * single operation.
     */
    QMutex _instancesMutex;

    /*!
     * \brief A string containing the name of the configuration section to use.
     */
    QString _section;
};

template<typename T>
QSharedPointer<T> Builder::get(const char *name)
{
   QSharedPointer<T> object = get(name).objectCast<T>();

   if (!object)
   {
        QString message = QString("Type does not implement the requested service.");
        throw BuilderException(message);
   }
   return object;
}

template<typename T>
QSharedPointer<T> Builder::get()
{
   const char *name = qobject_interface_iid<T *>();
   return get<T>(name);
}
