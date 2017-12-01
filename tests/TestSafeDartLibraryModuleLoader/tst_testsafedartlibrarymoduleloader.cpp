/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: tst_testsafedartlibrarymoduleloader.cpp
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
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLibrary>
#include <QtTest>

#include <librarymoduleloader.h>

class TestSafeDartLibraryModuleLoader : public QObject
{
    Q_OBJECT

private slots:
    void testGetLoadedModules();
    void testLoadModule();
    void testLoadModulesInDir();
};

void TestSafeDartLibraryModuleLoader::testGetLoadedModules()
{
    // Arrange
    ModuleLoader::Module expectedModule;
    expectedModule.path = TESTLIB;
    expectedModule.name = "Test Module";
    expectedModule.version = "Test Version";

    LibraryModuleLoader loader;
    loader.modules.append(expectedModule);

    // Act
    QList<ModuleLoader::Module> result = loader.getLoadedModules();

    // Assert
    QCOMPARE(result.size(), 1);
    QCOMPARE(result[0].path, QString(TESTLIB));
    QCOMPARE(result[0].name, QString("Test Module"));
    QCOMPARE(result[0].version, QString("Test Version"));
}

void TestSafeDartLibraryModuleLoader::testLoadModule()
{
    // Arrange
    QFile systemFile(TESTLIB);
    QFileInfo systemFileInfo(systemFile);
    QString systemPath = systemFileInfo.absoluteFilePath();

    LibraryModuleLoader loader;

    // Act
    bool result = loader.loadModule(systemPath);

    // Assert
    QLibrary testLibrary(systemPath);
    QVERIFY2(result, "LibraryModuleLoader did not successfully load module.");
    QVERIFY2(testLibrary.isLoaded(), "LibraryModuleLoader did not load the module.");

    QList<ModuleLoader::Module> modules = loader.getLoadedModules();
    QCOMPARE(modules.size(), 1);
    QCOMPARE(modules[0].path, systemPath);
    QCOMPARE(modules[0].name, QString("Test Module"));
    QCOMPARE(modules[0].version, QString("Test Version"));
}

void TestSafeDartLibraryModuleLoader::testLoadModulesInDir()
{
    // Arrange
    QFile systemFile(TESTLIB);
    QFileInfo systemFileInfo(systemFile);

    QDir tempDir = QDir::temp();
    QDir localDir = tempDir.filePath("TestSafeDartLibraryModuleLoader");
    localDir.removeRecursively();
    tempDir.mkdir("TestSafeDartLibraryModuleLoader");

    QString localPath = localDir.filePath(systemFileInfo.fileName());
    systemFile.copy(localPath);

    LibraryModuleLoader loader;

    // Act
    int result = loader.loadModulesFromDir(localDir.path());

    // Assert
    localDir.removeRecursively();

    QLibrary testLibrary(localPath);
    QVERIFY2(result == 1, "LibraryModuleLoader did not report 1 successfully loaded module.");
    QVERIFY2(testLibrary.isLoaded(), "LibraryModuleLoader did not load the module.");

    QList<ModuleLoader::Module> modules = loader.getLoadedModules();
    QCOMPARE(modules.size(), 1);
    QCOMPARE(modules[0].path, localPath);
    QCOMPARE(modules[0].name, QString("Test Module"));
    QCOMPARE(modules[0].version, QString("Test Version"));
}

QTEST_MAIN(TestSafeDartLibraryModuleLoader)

#include "tst_testsafedartlibrarymoduleloader.moc"
