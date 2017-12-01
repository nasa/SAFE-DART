/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: tst_testsafedartsafeapplication.cpp
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
#include <QtTest>

#include <builder.h>
#include <memoryconfiguration.h>
#include <messagetracker.h>
#include <mockapplication.h>
#include <mockmoduleloader.h>
#include <safeapplication.h>
#include <safeconfiguration.h>

class TestSafeDartSafeApplication : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();

    void testMainApplicationInvalid();
    void testMainConfigurationInvalid();
    void testMainModuleLoaderInvalid();
    void testMainNoApplication();
    void testMainSuccess();

    void cleanupTestCase();

private:
    QSharedPointer<MockApplication> mockApplication();
    QSharedPointer<Configuration> mockConfiguration();
    QSharedPointer<MockModuleLoader> mockModuleLoader();

    QScopedPointer<SafeApplication> _application;
    QScopedPointer<Builder> _builder;
    QScopedPointer<MessageTracker> _messageTracker;
    QSharedPointer<MockApplication> _mockApplication;
    QSharedPointer<MemoryConfiguration> _mockConfiguration;
    QSharedPointer<MockModuleLoader> _mockModuleLoader;
};

void TestSafeDartSafeApplication::initTestCase()
{
    _messageTracker.reset(new MessageTracker);
}

void TestSafeDartSafeApplication::init()
{
    _application.reset();
    _builder.reset(new Builder);
    _application.reset(new SafeApplication(_builder.data()));

    _messageTracker->clear();
    _mockApplication.reset();
    _mockConfiguration.reset();
    _mockModuleLoader.reset();
}

void TestSafeDartSafeApplication::cleanupTestCase()
{
    _messageTracker.reset();
}

QSharedPointer<MockApplication> TestSafeDartSafeApplication::mockApplication()
{
    if(!_mockApplication)
    {
        _mockApplication.reset(new MockApplication);
        _builder->provide("Application", _mockApplication);
    }
    return _mockApplication;
}

QSharedPointer<Configuration> TestSafeDartSafeApplication::mockConfiguration()
{
    if(!_mockConfiguration)
    {
        _mockConfiguration.reset(new MemoryConfiguration);
        _builder->provide("SafeConfiguration", _mockConfiguration);
    }
    return _mockConfiguration;
}

QSharedPointer<MockModuleLoader> TestSafeDartSafeApplication::mockModuleLoader()
{
    if(!_mockModuleLoader)
    {
        _mockModuleLoader.reset(new MockModuleLoader);
        _builder->provide("ModuleLoader", _mockModuleLoader);
    }
    return _mockModuleLoader;
}

void TestSafeDartSafeApplication::testMainApplicationInvalid()
{
    char a[] = "safedart";
    char b[] = "Application";
    char *argv[] = { a, b };

    mockConfiguration();
    mockModuleLoader();

    int result = _application->main(2, argv);

    QVERIFY2(result == 1, "main did not return error code when Application was not found");
}

void TestSafeDartSafeApplication::testMainConfigurationInvalid()
{
    char a[] = "safedart";
    char b[] = "Application";
    char *argv[] = { a, b };

    mockApplication()->_result = 0;
    mockModuleLoader();

    QSharedPointer<QObject> allegedConfiguration = QSharedPointer<QObject>::create();
    _builder->provide("SafeConfiguration", allegedConfiguration);

    int result = _application->main(2, argv);

    QVERIFY2(result == 0, "main returned error code.");
    QVERIFY2(mockApplication()->_calls.size() == 1, "main did not call Application.");
    QVERIFY2(_builder->configuration() == nullptr, "main set a Configuration on the Builder.");
}

void TestSafeDartSafeApplication::testMainModuleLoaderInvalid()
{
    char a[] = "safedart";
    char b[] = "Application";
    char *argv[] = { a, b };

    mockApplication()->_result = 0;
    mockConfiguration();

    int result = _application->main(2, argv);

    QVERIFY2(result == 0, "main returned error code.");
    QVERIFY2(mockApplication()->_calls.size() == 1, "main did not call Application.");
}

void TestSafeDartSafeApplication::testMainNoApplication()
{
    char a[] = "safedart";
    char *argv[] = { a };

    int result = _application->main(1, argv);

    QVERIFY2(result == 1, "main returned wrong result.");
	QVERIFY2(_messageTracker->size() == 1, "main did not print anything.");
	QVERIFY2(_messageTracker->get(0).message == "USAGE: safedart <application>", "main did not print the correct message.");
	QVERIFY2(_messageTracker->get(0).type == QtCriticalMsg, "main did not print the correct message.");
}

void TestSafeDartSafeApplication::testMainSuccess()
{
    char a[] = "safedart";
    char b[] = "-f";
    char c[] = "test.ini";
    char d[] = "-s";
    char e[] = "section";
    char f[] = "Application";
    char *argv[] = { a, b, c, d, e, f };

    mockApplication()->_result = 387;
    mockConfiguration()->set("section/@module_dirs", QStringList { "foo", "bar" });
    mockConfiguration()->set("section/@module_files", QStringList { "baz", "qux" });
    mockModuleLoader()->_loadModuleResult = true;
    mockModuleLoader()->_loadModulesFromDirResult = 3;

    int result = _application->main(6, argv);

    QVERIFY2(result == mockApplication()->_result, "main returned wrong result.");
    QVERIFY2(mockApplication()->_calls.size() == 1, "main did not call Application.");
    QVERIFY2(mockApplication()->_calls[0].argc == 6, "main called Application with wrong argc.");
    QVERIFY2(mockApplication()->_calls[0].argv == argv, "main called Application with wrong argv.");

    QVERIFY2(_builder->configuration() == mockConfiguration(), "main set wrong Configuration.");
    QVERIFY2(_builder->section() == "section", "main set wrong section on Builder");
    QVERIFY2(SafeConfiguration::getFile() == "test.ini", "main set wrong file on SafeConfiguration");

    QVERIFY2(mockModuleLoader()->_directories.size() == 2, "main did not load modules from a directory.");
    QVERIFY2(mockModuleLoader()->_directories.contains("foo"), "main did not load modules from ./mod.");
    QVERIFY2(mockModuleLoader()->_directories.contains("bar"), "main did not load modules from ./mod.");
    QVERIFY2(mockModuleLoader()->_libraries.size() == 2, "main did not load modules by path.");
    QVERIFY2(mockModuleLoader()->_libraries.contains("baz"), "main did not load modules from ./mod.");
    QVERIFY2(mockModuleLoader()->_libraries.contains("qux"), "main did not load modules from ./mod.");
}

QTEST_GUILESS_MAIN(TestSafeDartSafeApplication)

#include "tst_testsafedartsafeapplication.moc"
