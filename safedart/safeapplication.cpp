/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: safeapplication.cpp
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

#include "safeapplication.h"

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
#include <application.h>
#include <configuration.h>
#include <moduleloader.h>
#include <safeconfiguration.h>

// ********************************************************************** */
SafeApplication::SafeApplication(Builder *builder, QObject *parent) :
// ********************************************************************** */
    QObject(parent)
{
    _builder = builder;
} // SafeApplication::SafeApplication(Builder *builder, QObject *parent)

// ********************************************************************** */
void SafeApplication::loadedModule(QString name, QString description, QString version)
// ********************************************************************** */
{
    QByteArray rawDescription = description.toUtf8();
    QByteArray rawName        = name.toUtf8();
    QByteArray rawVersion     = version.toUtf8();

    qDebug("Loaded <%s> (\"%s\", version %s).", rawName.data(), rawDescription.data(), rawVersion.data());
} // end void SafeApplication::loadedModule(QString name, QString description, QString version)

// ********************************************************************** */
int SafeApplication::main(int argc, char **argv)
// ********************************************************************** */
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Hosts a software system constructed using the SAFE-DART "
                                     "framework.");

    QCommandLineOption fileOption(QStringList {"f", "file"},
                                  "The path to the configuration file to use for configuration.",
                                  "file", SafeConfiguration::getFile());
    parser.addOption(fileOption);

    parser.addHelpOption();

    QCommandLineOption sectionOption(QStringList {"s", "section"},
                                     "The section within the configuration file to read from.",
                                     "section", "safedart");
    parser.addOption(sectionOption);

    QCoreApplication app(argc, argv);
    parser.process(app);

    QStringList arguments = parser.positionalArguments();
    if(arguments.size() < 1)
    {
        qCritical("USAGE: %s <application>", argv[0]);
        return 1;
    }

    QByteArray application = arguments[0].toLocal8Bit();
    QString file = parser.value(fileOption);
    QString section = parser.value(sectionOption);
    SafeConfiguration::setFile(file);

    try
    {
        QSharedPointer<Configuration> configuration =
                _builder->get<Configuration>("SafeConfiguration");
        _builder->setConfiguration(configuration, section);
    }
    catch(const QException &e)
    {
        qWarning("Failed to load SAFE configuration: %s", e.what());
    }

    QSharedPointer<ModuleLoader> loader;
    try
    {
        int loaded = 0;
        loader = _builder->get<ModuleLoader>();

        QSharedPointer<QObject> loaderObject = loader.dynamicCast<QObject>();
        connect(loaderObject.data(), SIGNAL(loadedModule(QString,QString,QString)),
                this,                SLOT  (loadedModule(QString,QString,QString)));

        QSharedPointer<Configuration> configuration = _builder->configuration();
        if (configuration)
        {
            QStringList directories = configuration->get(section + "/@module_dirs").toStringList();
            for(int i = 0; i < directories.size(); i++)
            {
                loaded += loader->loadModulesFromDir(directories[i]);
            }

            QStringList files = configuration->get(section + "/@module_files").toStringList();
            for(int i = 0; i < files.size(); i++)
            {
                if(loader->loadModule(files[i]))
                    loaded++;
            }
        }

        qDebug("%d module(s) loaded.", loaded);
    }
    catch(const QException &e)
    {
        qWarning("Failed to load modules: %s", e.what());
    }

    try
    {
        QSharedPointer<Application> app = _builder->get<Application>(application);
        return app->main(argc, argv);
    }
    catch(const QException &e)
    {
        qCritical("Failed to run Application: %s", e.what());
        return 1;
    }
} // end int SafeApplication::main(int argc, char **argv)
