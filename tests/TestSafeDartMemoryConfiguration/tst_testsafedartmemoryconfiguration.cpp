/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: tst_testsafedartmemoryconfiguration.cpp
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

#include <memoryconfiguration.h>

class OpenMemoryConfiguration : public MemoryConfiguration
{
public:
    explicit OpenMemoryConfiguration(QObject *parent = 0) :
        MemoryConfiguration(parent)
    {}

    explicit OpenMemoryConfiguration(const QHash<QString, QVariant> &copy, QObject *parent = 0) :
        MemoryConfiguration(copy, parent)
    {}

    explicit OpenMemoryConfiguration(QHash<QString, QVariant> &&move, QObject *parent = 0) :
        MemoryConfiguration(qMove(move), parent)
    {}

    explicit OpenMemoryConfiguration(const MemoryConfiguration &copy, QObject *parent = 0) :
        MemoryConfiguration(copy, parent)
    {}

    explicit OpenMemoryConfiguration(MemoryConfiguration &&move, QObject *parent = 0) :
        MemoryConfiguration(qMove(move), parent)
    {}

    using MemoryConfiguration::_hash;
};

class TestSafeDartMemoryConfiguration : public QObject
{
    Q_OBJECT

private slots:
    void testConstructorEmpty();
    void testConstructorCopyHash();
    void testConstructorMoveHash();
    void testConstructorCopyMemoryConfiguration();
    void testConstructorMoveMemoryConfiguration();

    void testClear();
    void testGetAbsent();
    void testGetPresent();
    void testRemoveAbsent();
    void testRemovePresent();
    void testSetAbsent();
    void testSetPresent();

private:
    QHash<QString, QVariant> sampleData();
};

QHash<QString, QVariant> TestSafeDartMemoryConfiguration::sampleData()
{
    QHash<QString, QVariant> data;
    data.insert("a", true);
    data.insert("b", 12);
    data.insert("c", 3.14);
    data.insert("d", "foo");
    return data;
}

void TestSafeDartMemoryConfiguration::testConstructorEmpty()
{
    OpenMemoryConfiguration configuration(this);

    QVERIFY2(configuration._hash.empty(), "MemoryConfiguration initialized with values.");
    QVERIFY2(configuration.parent() == this, "MemoryConfiguration initialized with wrong parent.");
}

void TestSafeDartMemoryConfiguration::testConstructorCopyHash()
{
    QHash<QString, QVariant> data = sampleData();

    OpenMemoryConfiguration result(data, this);

    QVERIFY2(result._hash == sampleData(), "MemoryConfiguration initialized with wrong values.");
    QVERIFY2(result.parent() == this, "MemoryConfiguration initialized with wrong parent.");
}

void TestSafeDartMemoryConfiguration::testConstructorMoveHash()
{
    QHash<QString, QVariant> data = sampleData();

    OpenMemoryConfiguration result(qMove(data), this);

    QVERIFY2(result._hash == sampleData(), "MemoryConfiguration initialized with wrong values.");
    QVERIFY2(result.parent() == this, "MemoryConfiguration initialized with wrong parent.");
}

void TestSafeDartMemoryConfiguration::testConstructorCopyMemoryConfiguration()
{
    OpenMemoryConfiguration source;
    source._hash = sampleData();

    OpenMemoryConfiguration result(source, this);

    QVERIFY2(result._hash == sampleData(), "MemoryConfiguration initialized with wrong values.");
    QVERIFY2(result.parent() == this, "MemoryConfiguration initialized with wrong parent.");
}

void TestSafeDartMemoryConfiguration::testConstructorMoveMemoryConfiguration()
{
    OpenMemoryConfiguration source;
    source._hash = sampleData();

    OpenMemoryConfiguration result(qMove(source), this);

    QVERIFY2(result._hash == sampleData(), "MemoryConfiguration initialized with wrong values.");
    QVERIFY2(result.parent() == this, "MemoryConfiguration initialized with wrong parent.");
}

void TestSafeDartMemoryConfiguration::testClear()
{
    OpenMemoryConfiguration configuration(sampleData());

    configuration.clear();

    QVERIFY2(configuration._hash.empty(), "MemoryConfiguration did not clear all keys.");
}

void TestSafeDartMemoryConfiguration::testGetAbsent()
{
    OpenMemoryConfiguration configuration(sampleData());

    QVariant result = configuration.get("e", "default");

    QVERIFY2(result == "default", "MemoryConfiguration did not return default value.");
}

void TestSafeDartMemoryConfiguration::testGetPresent()
{
    OpenMemoryConfiguration configuration(sampleData());

    QVariant result = configuration.get("d", "default");

    QVERIFY2(result == "foo", "MemoryConfiguration did not return assigned value.");
}

void TestSafeDartMemoryConfiguration::testRemoveAbsent()
{
    OpenMemoryConfiguration configuration(sampleData());

    configuration.remove("e");

    QVERIFY2(configuration._hash.size() == 4, "MemoryConfiguration removed incorrect key.");
}

void TestSafeDartMemoryConfiguration::testRemovePresent()
{
    OpenMemoryConfiguration configuration(sampleData());

    configuration.remove("d");

    QVERIFY2(configuration._hash.size() == 3, "MemoryConfiguration did not remove one value.");
    QVERIFY2(!configuration._hash.contains("d"), "MemoryConfiguration removed incorrect key.");
}

void TestSafeDartMemoryConfiguration::testSetAbsent()
{
    OpenMemoryConfiguration configuration(sampleData());

    configuration.set("e", "bar");

    QVERIFY2(configuration._hash.size() == 5, "MemoryConfiguration did not add one value.");
    QVERIFY2(configuration._hash.value("e") == "bar", "MemoryConfiguration added incorrect key/value.");
}

void TestSafeDartMemoryConfiguration::testSetPresent()
{
    OpenMemoryConfiguration configuration(sampleData());

    configuration.set("d", "bar");

    QVERIFY2(configuration._hash.size() == 4, "MemoryConfiguration changed the number of entries.");
    QVERIFY2(configuration._hash.value("d") == "bar", "MemoryConfiguration set incorrect value.");
}

QTEST_GUILESS_MAIN(TestSafeDartMemoryConfiguration)

#include "tst_testsafedartmemoryconfiguration.moc"
