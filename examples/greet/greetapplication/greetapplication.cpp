/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Software Architecture Framework for Extensibility using Dynamic Assembly of Reflective Types (SAFE-DART)
**
** Filename: greetapplication.cpp
** File Date: 20171201
**
** Authors **
** Author: Jordan R. Higgins (JRH)
**
** Revision History:
**   <Date> <Name of Change Agent>
**   Description:
**     - Bulleted list of changes.
**
** Copyright © 2017 United States Government as represented by NASA Glenn Research Center. No copyright is claimed in the United States under Title 17, U.S.Code. All Other Rights Reserved. See LICENSE.txt in the root of the repository for more details.
**
********************************************************************** */
#include "greetapplication.h"

GreetApplication::GreetApplication(Builder *builder)
    : _builder(builder)
{
}

int GreetApplication::main(int argc, char **argv)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    QSharedPointer<Greeter> greeter = _builder->get<Greeter>();
    greeter->greet();
    return 0;
}
