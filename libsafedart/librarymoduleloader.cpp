/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: librarymoduleloader.cpp
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

#include "librarymoduleloader.h"

#include <QDir>
#include <QLibrary>
#include <QSet>

// ********************************************************************** */
LibraryModuleLoader::LibraryModuleLoader(QObject *parent) : QObject(parent)
// ********************************************************************** */
{
} // LibraryModuleLoader::LibraryModuleLoader(QObject *parent)

// ********************************************************************** */
LibraryModuleLoader::~LibraryModuleLoader()
// ********************************************************************** */
{
} // LibraryModuleLoader::~LibraryModuleLoader()

// ********************************************************************** */
QList<ModuleLoader::Module> LibraryModuleLoader::getLoadedModules() const
// ********************************************************************** */
{
    QReadLocker lock(&modulesLock); Q_UNUSED(lock);
    return modules;

} // QList<ModuleLoader::Module> LibraryModuleLoader::getLoadedModules() const

// ********************************************************************** */
bool LibraryModuleLoader::loadModule(const QString &name)
// ********************************************************************** */
{
    QLibrary library(name);
    if (!library.load())
    {
        return false;
    }

    typedef const char *(*InfoFunction)();
    InfoFunction descriptionFunction = (InfoFunction)library.resolve("safedart_module_description");
    InfoFunction versionFunction = (InfoFunction)library.resolve("safedart_module_version");

    Module module;
    module.name = (descriptionFunction) ? (descriptionFunction()) : QString();
    module.path = name;
    module.version = (versionFunction) ? (versionFunction()) : QString();

    {
        QWriteLocker lock(&modulesLock); Q_UNUSED(lock);
        modules.append(module);
    }

    emit loadedModule(module.path, module.name, module.version);
    return true;

} // bool LibraryModuleLoader::loadModule(const QString &name)

// ********************************************************************** */
int LibraryModuleLoader::loadModulesFromDir(const QString &path)
// ********************************************************************** */
{
    QSet<QString> modulePaths;

    QDir moduleDir(path);
    moduleDir.setFilter(QDir::Files);
    for (QFileInfo fileInfo : moduleDir.entryInfoList())
    {
        QString filePath = fileInfo.canonicalFilePath();
        if (QLibrary::isLibrary(filePath))
        {
            modulePaths.insert(filePath);
        }
    }

    int modulesLoaded = 0;

    for (QString modulePath : modulePaths)
    {
        if (loadModule(modulePath))
        {
            modulesLoaded++;
        }
    }

    return modulesLoaded;

} // int LibraryModuleLoader::loadModulesFromDir(const QString &path)
