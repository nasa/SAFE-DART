/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: moduleloader.h
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

#include <QList>
#include <QObject>
#include <QString>

/*!
 * \brief An interface for loading modules.
 *
 * Because of how SAFE-DART works, loading a module is sufficient to use it; no
 * additional interaction is required. Once a module is loaded, all of its
 * reflective types are available for use via a \c Builder.
 *
 * \ingroup SAFE-DART-Framework
 */
class ModuleLoader
{
public:
    /*!
     * \brief Information about a module.
     */
    struct Module
    {
        /*!
         * \brief The name of the module, as provided by the module itself.
         *
         * This may be a NULL string if the module did not provide a name.
         */
        QString name;

        /*!
         * \brief The absolute path to the shared library file for the module.
         */
        QString path;

        /*!
         * \brief The version of the module, as provided by the module itself.
         *
         * This may be a NULL string if the module did not provide a version.
         */
        QString version;
    };

    virtual ~ModuleLoader() {}

    /*!
     * \brief Returns information about all loaded modules.
     *
     * The list will contain the modules in the order that they were loaded.
     */
    virtual QList<Module> getLoadedModules() const = 0;

    /*!
     * \brief Loads a single module by name or path.
     *
     * \param name The name of the module to load. This may be a platform-
     * agnostic name, in which case appropriate PATH directories are searched,
     * or a complete file path to a module.
     *
     * \retval true The module was loaded successfully.
     * \retval false The module was not able to be loaded; no further
     * information is available.
     */
    virtual bool loadModule(const QString &name) = 0;

    /*!
     * \brief Loads all of the modules in a specific directory.
     *
     * \param path The path to the directory from which to load modules.
     *
     * \return The number of modules successfully loaded.
     */
    virtual int loadModulesFromDir(const QString &path) = 0;

signals:
    /*!
     * \brief Emitted when a module is loaded successfully.
     *
     * \param name The name or full path of the module.
     * \param description The description of the module, as provided by the
     * module, or a NULL string if it did not provide one.
     * \param version The version of the module, as provided by the module, or a
     * NULL string if it did not provide one.
     */
    virtual void loadedModule(QString name, QString description, QString version) = 0;
};

Q_DECLARE_INTERFACE(ModuleLoader, "ModuleLoader")
