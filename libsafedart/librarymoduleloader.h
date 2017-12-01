/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: librarymoduleloader.h
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

#include <QObject>
#include <QReadWriteLock>
#include <moduleloader.h>
#include <reflectable.h>

/*!
 * \brief An implementation of ModuleLoader using QLibrary to load modules.
 *
 * This implementation loads shared libraries as modules. No direct function
 * calls are made into the shared library; all initialization must be done
 * automatically. See \c Reflectable for a simple way of registering reflective
 * types automatically.
 *
 * \ingroup SAFE-DART-Framework
 */
class LibraryModuleLoader :
    public QObject,
    public ModuleLoader,
    public Reflectable<LibraryModuleLoader>
{
    Q_OBJECT
    Q_INTERFACES(ModuleLoader)

public:
    /*!
     * \brief Creates a LibraryModuleLoader.
     *
     * \param parent The parent QObject of this QObject.
     */
    Q_INVOKABLE explicit LibraryModuleLoader(QObject *parent = 0);
    ~LibraryModuleLoader();

    QList<Module> getLoadedModules() const override;
    bool loadModule(const QString &name) override;
    int  loadModulesFromDir(const QString &path) override;

signals:
    void loadedModule(QString name, QString description, QString version) override;

protected:
    QList<Module> modules;
    mutable QReadWriteLock modulesLock;
};
