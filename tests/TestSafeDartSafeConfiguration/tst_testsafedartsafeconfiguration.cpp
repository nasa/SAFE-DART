/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: tst_testsafedartsafeconfiguration.cpp
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
#include <QFileInfo>
#include <QUuid>
#include <QtTest>

#include <safeconfiguration.h>

class OpenSafeConfiguration :
        public SafeConfiguration
{
public:
    OpenSafeConfiguration(QObject *parent = 0) :
        SafeConfiguration(parent)
    {
    }

    using SafeConfiguration::_file;
    using SettingsConfiguration::_settings;
};

class TestSafeDartSafeConfiguration : public QObject
{
    Q_OBJECT

private slots:
    void testSettings();
};

void TestSafeDartSafeConfiguration::testSettings()
{
    QString file = QUuid::createUuid().toString();
    SafeConfiguration::setFile(file);

    OpenSafeConfiguration configuration(this);

    QVERIFY2(configuration.parent() == this, "SafeConfiguration did not set parent.");
    QVERIFY2(configuration._settings != nullptr, "SafeConfiguration did not set QSettings.");
    QVERIFY2(QFileInfo(configuration._settings->fileName()) == QFileInfo(file), "SafeConfiguration did not set QSettings with correct file name.");
    QVERIFY2(configuration._settings->format() == QSettings::IniFormat, "SafeConfiguration did not set QSettings with correct format.");
    QVERIFY2(configuration._settings->group() == "", "SafeConfiguration did not set QSettings with correct group.");
}

QTEST_GUILESS_MAIN(TestSafeDartSafeConfiguration)

#include "tst_testsafedartsafeconfiguration.moc"
