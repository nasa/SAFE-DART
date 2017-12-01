#pragma once

#include <greeter.h>

class MockGreeter :
        public QObject,
        public Greeter
{
    Q_OBJECT
    Q_INTERFACES(Greeter)

public:
    MockGreeter(QObject *parent = 0);

    void greet() override;

    int _greetings;
};
