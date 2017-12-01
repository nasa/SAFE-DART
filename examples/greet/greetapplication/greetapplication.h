/* **********************************************************************
**
** Developed for NASA Glenn Research Center
** By: Flight Software Branch (LSS)
**
** Project: Software Architecture Framework for Extensibility using Dynamic Assembly of Reflective Types (SAFE-DART)
**
** Filename: greetapplication.h
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
#pragma once

#include <application.h>
#include <builder.h>
#include <greeter.h>
#include <reflectable.h>

class GreetApplication : public QObject, public Application, public Reflectable<GreetApplication>
{
    Q_OBJECT
    Q_INTERFACES(Application)

public:
    Q_INVOKABLE GreetApplication(Builder *builder);
    int main(int argc, char **argv) override;

private:
    Builder *_builder;
};
