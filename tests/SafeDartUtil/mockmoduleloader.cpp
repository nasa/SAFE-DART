/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: mockmoduleloader.cpp
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
********************************************************************** */
#include "mockmoduleloader.h"

MockModuleLoader::MockModuleLoader()
{
    _loadModulesFromDirResult = 0;
    _loadModuleResult = false;
}

QList<ModuleLoader::Module> MockModuleLoader::getLoadedModules() const
{
    return _getLoadedModulesResult;
}

int MockModuleLoader::loadModulesFromDir(const QString &path)
{
    _directories.push_back(path);
    return _loadModulesFromDirResult;
}

bool MockModuleLoader::loadModule(const QString &name)
{
    _libraries.push_back(name);
    return _loadModuleResult;
}
