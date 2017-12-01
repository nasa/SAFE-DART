#include "mockgreeter.h"

MockGreeter::MockGreeter(QObject *parent) :
    QObject(parent),
    _greetings(0)
{
}

void MockGreeter::greet()
{
    _greetings++;
}
