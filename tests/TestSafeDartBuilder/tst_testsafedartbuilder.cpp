/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Flow Boiling and Condensation Experiment (FBCE)
** Candidate for GOTS reuse once FBCE has completed V&V testing
**
** Filename: tst_testsafedartbuilder.cpp
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
#include <QCoreApplication>
#include <QThread>
#include <QUuid>
#include <QtTest>

#include <builder.h>
#include <memoryconfiguration.h>
#include <openbuilder.h>

class TestSafeDartBuilder : public QObject
{
    Q_OBJECT

private slots:
    void init();

    void testConfiguration();
    void testGetNewFromConfiguration();
    void testGetNewNotInvokable();
    void testGetNewWithBuilder();
    void testGetNewWithMissing();
    void testGetNewWithNone();
    void testGetRecursive();
    void testGetReplaceExpired();
    void testGetTypedNewCorrectType();
    void testGetTypedNewWrongType();
    void testGetUseConcurrent();
    void testGetUseExisting();
    void testProvideAddNew();
    void testProvideReplaceExisting();
    void testProvideReplaceExpired();
    void testSetConfiguration();

    void benchmarkGetExisting();
    void benchmarkGetNew();
    void benchmarkProvide();

private:
    QScopedPointer<OpenBuilder> _builder;
};

class TestObjectNotInvokable : public QObject
{
    Q_OBJECT

public:
    TestObjectNotInvokable(QObject *parent = 0) :
        QObject(parent)
    {
    }
};

Q_DECLARE_INTERFACE(TestObjectNotInvokable, "TestObjectNotInvokable")

class TestObjectInvokableWithBuilder : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE TestObjectInvokableWithBuilder(QObject *parent = 0) :
        QObject(parent),
        builder(nullptr)
    {
    }

    Q_INVOKABLE explicit TestObjectInvokableWithBuilder(Builder *builder, QObject *parent = 0) :
        QObject(parent),
        builder(builder)
    {
    }

    Builder *builder;
};

Q_DECLARE_INTERFACE(TestObjectInvokableWithBuilder, "TestObjectInvokableWithBuilder")

class TestObjectInvokableWithNone : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit TestObjectInvokableWithNone(QObject *parent = 0) :
        QObject(parent)
    {
    }
};

Q_DECLARE_INTERFACE(TestObjectInvokableWithNone, "TestObjectInvokableWithNone")

class TestObjectRecursive : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit TestObjectRecursive(Builder *builder, QObject *parent = 0) :
        QObject(parent)
    {
        other = builder->get("TestObjectInvokableWithNone");
    }

    QSharedPointer<QObject> other;
};

Q_DECLARE_INTERFACE(TestObjectRecursive, "TestObjectRecursive")

void TestSafeDartBuilder::init()
{
    _builder.reset(new OpenBuilder);

    qMetaTypeId<TestObjectInvokableWithBuilder *>();
    qMetaTypeId<TestObjectInvokableWithNone *>();
    qMetaTypeId<TestObjectNotInvokable *>();
    qMetaTypeId<TestObjectRecursive *>();
}

void TestSafeDartBuilder::testConfiguration()
{
    QSharedPointer<Configuration> configuration(new MemoryConfiguration);
    _builder->_configuration = configuration;

    QSharedPointer<Configuration> result = _builder->configuration();
    QVERIFY2(result == configuration, "Returned wrong object");
}

void TestSafeDartBuilder::testGetNewFromConfiguration()
{
    QString section = QUuid::createUuid().toString(); 

    QSharedPointer<Configuration> configuration(new MemoryConfiguration);
    configuration->set(section + "/TestObject", "TestObjectInvokableWithNone");
    _builder->setConfiguration(configuration, section);

    QSharedPointer<TestObjectInvokableWithNone> result = _builder
            ->get("TestObject")
            .objectCast<TestObjectInvokableWithNone>();
    QVERIFY2(result, "Failed to create object");
}

void TestSafeDartBuilder::testGetNewNotInvokable()
{
    QVERIFY_EXCEPTION_THROWN(_builder->get("TestObjectNotInvokable"), BuilderException);
}

void TestSafeDartBuilder::testGetNewWithBuilder()
{
    QSharedPointer<TestObjectInvokableWithBuilder> result = _builder
            ->get("TestObjectInvokableWithBuilder")
            .objectCast<TestObjectInvokableWithBuilder>();

    OpenBuilder::Instance &instance = _builder->_instances["TestObjectInvokableWithBuilder"];
    QSharedPointer<QObject> cached = instance._reference;
    QVERIFY2(result, "Failed to create object");
    QVERIFY2(result->builder == _builder.data(), "Object was not created with the Builder");
    QVERIFY2(cached == result, "Builder did not store the created object");
}

void TestSafeDartBuilder::testGetNewWithMissing()
{
    QVERIFY_EXCEPTION_THROWN(_builder->get<TestObjectInvokableWithNone>("TestObjectNonexistent"), BuilderException);
}

void TestSafeDartBuilder::testGetNewWithNone()
{
    QSharedPointer<TestObjectInvokableWithNone> result = _builder
            ->get("TestObjectInvokableWithNone")
            .objectCast<TestObjectInvokableWithNone>();

    OpenBuilder::Instance &instance = _builder->_instances["TestObjectInvokableWithNone"];
    QSharedPointer<QObject> cached = instance._reference;
    QVERIFY2(result, "Failed to create object");
    QVERIFY2(cached == result, "Builder did not store the created object");
}

void TestSafeDartBuilder::testGetRecursive()
{
    class RecursiveThread : public QThread
    {
    public:
        RecursiveThread(Builder *builder, QObject *parent = 0) :
            QThread(parent),
            builder(builder)
        {
        }

    protected:
        void run() override
        {
            builder->get<TestObjectRecursive>();
        }

        Builder *builder;
    } thread(_builder.data());
    thread.start();

    QVERIFY2(thread.wait(5000), "Getting objects from the Builder recursively caused a deadlock");;

    thread.terminate();
    thread.wait();
}

void TestSafeDartBuilder::testGetReplaceExpired()
{
    OpenBuilder::Instance &instance = _builder->_instances["TestObjectInvokableWithNone"];
    instance._reference = QWeakPointer<QObject>();

    QSharedPointer<QObject> result = _builder
            ->get("TestObjectInvokableWithNone")
            .objectCast<TestObjectInvokableWithNone>();

    QSharedPointer<QObject> cached = instance._reference;
    QVERIFY2(result, "Builder did not create a new object");
    QVERIFY2(cached == result, "Builder did not store the created object");
}

void TestSafeDartBuilder::testGetTypedNewCorrectType()
{
    QSharedPointer<TestObjectInvokableWithNone> result = _builder
            ->get<TestObjectInvokableWithNone>();

    OpenBuilder::Instance &instance = _builder->_instances["TestObjectInvokableWithNone"];
    QSharedPointer<QObject> cached = instance._reference;
    QVERIFY2(result, "Builder did not create a new object");
    QVERIFY2(cached == result, "Builder did not store the created object");
}

void TestSafeDartBuilder::testGetTypedNewWrongType()
{
    QVERIFY_EXCEPTION_THROWN(_builder->get<TestObjectInvokableWithNone>("TestObjectInvokableWithBuilder"), BuilderException);
}

void TestSafeDartBuilder::testGetUseConcurrent()
{
    class ConcurrentThread : public QThread
    {
    public:
        ConcurrentThread(Builder *builder, QObject *parent = 0) :
            QThread(parent),
            builder(builder)
        {
        }

        QSharedPointer<QObject> result;

    protected:
        void run() override
        {
            result = builder->get("TestObjectInvokableWithNone");
        }

        Builder *builder;
    } thread(_builder.data());

    OpenBuilder::Instance &instance = _builder->_instances["TestObjectInvokableWithNone"];
    QMutexLocker lock(&instance._referenceMutex);

    thread.start();
    QThread::msleep(100);

    QSharedPointer<QObject> expected = QSharedPointer<TestObjectInvokableWithNone>::create();
    instance._reference = expected;
    lock.unlock();

    thread.wait();

    QSharedPointer<QObject> result = thread.result;
    QVERIFY2(expected == result, "Builder did not use concurrently-created object");
}

void TestSafeDartBuilder::testGetUseExisting()
{
    QSharedPointer<QObject> initial = QSharedPointer<TestObjectInvokableWithNone>::create();
    OpenBuilder::Instance &instance = _builder->_instances["TestObjectInvokableWithNone"];
    instance._reference = initial;

    QSharedPointer<QObject> result = _builder->get("TestObjectInvokableWithNone");
    QVERIFY2(initial == result, "Builder did not use existing object");

    QSharedPointer<Configuration> configuration(new MemoryConfiguration);
    configuration->set("safedart/TestObject", "TestObjectInvokableWithNone");
    _builder->setConfiguration(configuration);

    result = _builder->get("TestObject");
    QVERIFY2(initial == result, "Builder did not use existing object");
}

void TestSafeDartBuilder::testProvideAddNew()
{
    QSharedPointer<QObject> given = QSharedPointer<TestObjectInvokableWithNone>::create();
    _builder->provide("TestObjectInvokableWithNone", given);

    OpenBuilder::Instance &instance = _builder->_instances["TestObjectInvokableWithNone"];
    QSharedPointer<QObject> result = instance._reference;

    QVERIFY2(result == given, "Provide did not set new object");
}

void TestSafeDartBuilder::testProvideReplaceExisting()
{
    QSharedPointer<QObject> initial = QSharedPointer<TestObjectInvokableWithNone>::create();
    QSharedPointer<QObject> replacement = QSharedPointer<TestObjectInvokableWithNone>::create();

    OpenBuilder::Instance &instance = _builder->_instances["TestObjectInvokableWithNone"];
    instance._reference = initial;

    _builder->provide("TestObjectInvokableWithNone", replacement);

    QSharedPointer<QObject> result = instance._reference;
    QVERIFY2(result != initial, "Provide did not replace existing object");
    QVERIFY2(result == replacement, "Provide did not set new object");
}

void TestSafeDartBuilder::testProvideReplaceExpired()
{
    OpenBuilder::Instance &instance = _builder->_instances["TestObjectInvokableWithNone"];
    instance._reference = QWeakPointer<QObject>();

    QSharedPointer<QObject> replacement = QSharedPointer<TestObjectInvokableWithNone>::create();
    _builder->provide("TestObjectInvokableWithNone", replacement);

    QSharedPointer<QObject> result = instance._reference;
    QVERIFY2(result == replacement, "Provide did not set new object");
}

void TestSafeDartBuilder::testSetConfiguration()
{
    QSharedPointer<Configuration> configuration(new MemoryConfiguration);
    QString section = QUuid::createUuid().toString();

    _builder->setConfiguration(configuration, section);

    QVERIFY2(_builder->_configuration == configuration, "Did not set configuration");
    QVERIFY2(_builder->_section == section, "Did not set section");
}

void TestSafeDartBuilder::benchmarkGetExisting()
{
    QSharedPointer<TestObjectInvokableWithNone> existing = QSharedPointer<TestObjectInvokableWithNone>::create();
    _builder->provide("TestObjectInvokableWithNone", existing);

    QBENCHMARK
    {
        _builder->get("TestObjectInvokableWithNone");
    }
}

void TestSafeDartBuilder::benchmarkGetNew()
{
    QBENCHMARK
    {
        _builder->get("TestObjectInvokableWithNone");
    }
}

void TestSafeDartBuilder::benchmarkProvide()
{
    QSharedPointer<TestObjectInvokableWithNone> existing = QSharedPointer<TestObjectInvokableWithNone>::create();

    QBENCHMARK
    {
        _builder->provide("TestObjectInvokableWithNone", existing);
    }
}

QTEST_GUILESS_MAIN(TestSafeDartBuilder)

#include "tst_testsafedartbuilder.moc"
