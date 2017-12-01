/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: tst_testsafedartsettingsconfiguration.cpp
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

#include <settingsconfiguration.h>

class TestSafeDartSettingsConfiguration : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();

    void testClear();
    void testGetAbsent();
    void testGetPresent();
    void testRemove();
    void testSet();

private:
    QScopedPointer<SettingsConfiguration> _configuration;
    QScopedPointer<QSettings> _settings;
};

void TestSafeDartSettingsConfiguration::initTestCase()
{
    _settings.reset(new QSettings("./test.ini", QSettings::IniFormat));
}

void TestSafeDartSettingsConfiguration::init()
{
    _settings->clear();
    _settings->setValue("a", true);
    _settings->setValue("b", 83);
    _settings->setValue("c", 2.718);
    _settings->setValue("d", "foo");

    _configuration.reset(new SettingsConfiguration(_settings.data()));
}

void TestSafeDartSettingsConfiguration::testClear()
{
    _configuration->clear();

    QStringList keys = _settings->allKeys();
    QVERIFY2(keys.empty(), "clear did not remove all keys.");
}

void TestSafeDartSettingsConfiguration::testGetAbsent()
{
    QVariant result = _configuration->get("e", "bar");

    QVERIFY2(result == "bar", "get did not return the default value.");
}

void TestSafeDartSettingsConfiguration::testGetPresent()
{
    QVariant result = _configuration->get("d", "bar");

    QVERIFY2(result == "foo", "get did not return the correct value.");
}

void TestSafeDartSettingsConfiguration::testRemove()
{
    _configuration->remove("d");

    QStringList keys = _settings->allKeys();
    QVERIFY2(keys.size() == 3, "remove did not remove exactly one key.");
    QVERIFY2(!keys.contains("d"), "remove did not remove the correct key.");
}

void TestSafeDartSettingsConfiguration::testSet()
{
    _configuration->set("e", "bar");

    QStringList keys = _settings->allKeys();
    QVERIFY2(keys.size() == 5, "set did not add exactly one key.");
    QVERIFY2(_settings->value("e") == "bar", "set did not add the correct key.");
}

QTEST_GUILESS_MAIN(TestSafeDartSettingsConfiguration)

#include "tst_testsafedartsettingsconfiguration.moc"
