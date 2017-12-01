/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: reflectable.h
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

#include <QMetaType>

/*!
 * \brief Automatically registers a type for reflection on initialization.
 *
 * Reflection is the ability for software to get information about types that
 * exist within the software at run-time. This can be used for many things,
 * including getting information about what constructors, functions, and
 * properties are available on a type at run-time, as well as instantiating
 * types dynamically at run-time.
 *
 * In order for a custom type to be usable via reflection, it must be registered
 * with Qt by calling <tt>qMetaTypeId<T>()</tt> for that type. This class
 * provides a way of doing so automatically at initialization time, with
 * minimal extra code.
 *
 * In order to use this class, simply extend any class T which should be
 * accessible via reflection from <tt>Reflectable&lt;T&gt;</tt>.
 *
 * \warning This will always work when the subclass is compiled as part of an
 * executable or shared library. However, it will only work in a static library
 * if the class is referenced directly by other code that is in use. Otherwise,
 * the linker will eliminate initialization of the unused class and it will not
 * be available through reflection.
 *
 * \ingroup SAFE-DART-Framework
 */
template<typename T>
class Reflectable
{
public:
    /*!
     * \brief Ensures that \c _id is not compiled out.
     *
     * In order to ensure that \c _id is initialized as it should be, it must be
     * used somewhere, otherwise the compiler may omit it entirely. Its value
     * isn't important, so marking it as unused is sufficient.
     */
    Reflectable()
    {
        Q_UNUSED(_id);
    }

private:
    /*!
     * \brief The meta type ID for <tt>T *</tt>.
     *
     * The value of this isn't important, and isn't used for anything. The
     * initialization process itself is what is important. Getting the meta type
     * ID registers the meta type for <tt>T *</tt>, which allows it to be used
     * via reflection.
     */
    static const int _id;
};

template<typename T> const int Reflectable<T>::_id = qMetaTypeId<T *>();
