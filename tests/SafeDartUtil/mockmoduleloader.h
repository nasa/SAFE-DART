/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: mockmoduleloader.h
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

#include <QList>
#include <QObject>
#include <moduleloader.h>

class MockModuleLoader :
        public QObject,
        public ModuleLoader
{
    Q_OBJECT
    Q_INTERFACES(ModuleLoader)

public:
    MockModuleLoader();

    QList<Module> getLoadedModules() const override;
    int loadModulesFromDir(const QString &path) override;
    bool loadModule(const QString &name) override;

    QList<QString> _directories;
    QList<QString> _libraries;
    QList<Module> _getLoadedModulesResult;
    int _loadModulesFromDirResult;
    bool _loadModuleResult;

signals:
    void loadedModule(QString name, QString description, QString version) override;
};
