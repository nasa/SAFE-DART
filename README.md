# Software Architecture Framework for Extensibility (SAFE-DART)

## What is SAFE-DART?
SAFE-DART is a simple framework for projects written in the C++ programming language and using the Qt framework. SAFE-DART simplifies the modularization of object-oriented software, allowing software implementations of the components of a software system to be provided at run-time by modules (shared libraries).

SAFE-DART is split into two parts: the SAFE-DART library (libsafedart) and the SAFE-DART executable (safedart). The library is linked to any projects which use SAFE-DART, and provides the necessary functionality for creating and using modules via SAFE-DART. The executable provides a way to start applications which are built as SAFE-DART modules, and its use is optional.

## Building SAFE-DART
In order to make use of SAFE-DART, the libsafedart (library) project must first be built. This will create a shared library which may be linked into any applications which are to use SAFE-DART. This can be done in two ways on Linux:

1. Via the GUI: Open libsafedart/libsafedart.pro in Qt Creator and build.
2. Via the terminal: `cd libsafedart && qmake libsafedart.pro && make`

This will create a libsafedart shared library in the bin directory located in the same directory as this README.

The safedart (executable) project may be built similarly:

1. Via the GUI: Open safedart/safedart.pro in Qt Creator and build.
2. Via the terminal: `cd safedart && qmake safedart.pro && make`

The safedart executable will also be placed in the bin directory.

## Using SAFE-DART
A sample project setup, which can be run via the SAFE-DART executable, can be found in the `examples/greet` directory of the repository.

### Project Setup
A project can be linked to libsafedart by changing the following in its project file:

1. Add the libsafedart directory to its INCLUDEPATH (`INCLUDEPATH += <path to SAFE-DART repository>/libsafedart`).
2. Add the library directory and name to LIBS (`LIBS += -L<path to SAFE-DART repository>/bin -lsafedart`).

The project may then include SAFE-DART headers and make use of its functionality.

### Getting a Builder
Depending, the Builder may be acquired in one of two ways:

1. Provided to user code via the SAFE-DART executable. This approach has two main advantages:
    1. The SAFE-DART configuration file can be changed via the command-line.
    2. The application code can easily be tested all the way up to its main entry point.
2. Created by the user code. This has the advantage of allowing the application more control of SAFE-DART's behavior.

### Defining an Interface
An interface to a class is shared by any binary which makes use of the type. It should be written in a header which is shared by all modules that make use of the component it represents. In general, interfaces should contain only pure virtual methods to reduce the possibility of different implementations within different modules, and should also have a virtual destructor. An abstract class that extends `QObject` (a Qt class) may also be used. 

The interface should also be declared as an interface via `Q_DECLARE_INTERFACE` after the class definition. An example is shown below.

```
#include <QObject>
class Greeter
{
public:
    virtual ~Greeter() {}
    virtual void greet() = 0;
};
Q_DECLARE_INTERFACE(Greeter, "Greeter")
```

### Defining an Implementation
An implementation of an interface works mostly as normal, with a few additions:

1. If the interface does not extend `QObject`, then the implementation must.
2. The implementation should extend `Reflectable<T>`, where `T` is the implementation type.
3. The interface must be specified via `Q_INTERFACES`.
4. The implementation must have one of the following explicit constructors, shown in order of preference:
    1. `Q_INVOKABLE T(Builder *)`
    2. `Q_INVOKABLE T()`

An example of an implementation of the `Greeter` interface above is shown below:

```
#include <iostream>
#include <builder.h>
#include <greeter.h>
#include <reflectable.h>
class EnglishGreeter : public QObject, public Greeter, public Reflectable<EnglishGreeter>
{
    Q_OBJECT
    Q_INTERFACES(Greeter)
public:
    Q_INVOKABLE EnglishGreeter() {}
    void greet() override
    {
        std::cout << "Hello world!" << std::endl;
    }
};
```

### Creating an Application
Creating an application that may be used via the SAFE-DART executable is done by defining an implementation of an interface provided by SAFE-DART. This interface is called `Application`, and it contains only a single method (with the same signature as the standard `main` function).

The application can use the `T(Builder *)` constructor to receive an instance of the `Builder` class. The `Builder` instance can then be used to get an implementation of an interface (in this case, `Greeter`).

Note that the `Builder` returns a `QSharedPointer` to the implementation, and will return the same pointer as long as it continues to exist. The `Builder` itself holds a weak reference, so the object will be freed automatically once the application no longer holds a pointer to it.

An example implementation of `Application` that does this is as follows:

```
#include <application.h>
#include <builder.h>
class GreetApplication : public QObject, public Application, public Reflectable<GreetApplication>
{
    Q_OBJECT
    Q_INTERFACES(Application)
public:
    Q_INVOKABLE GreetApplication(Builder *builder) : _builder(builder) {}
    int main(int argc, char **argv) override
    {
        QSharedPointer<Greeter> greeter = _builder->get<Greeter>();
        greeter->greet();
        return 0;
    }

private:
    Builder *_builder;
};
```

### Configuring SAFE-DART
SAFE-DART uses a configuration file for two things:

1. Locations to load modules from. Only applicable when using the SAFE-DART executable.
    1. The `@module_dirs` key lists directories from which to load all modules. 
    2. The `@module_files` key lists specific module files to load.
2. Implementations to use for interfaces. The key is the interface name, the value is the implementation name. Always used by the `Builder`.

For the application above, the configuration file should contain the following:

```
[safedart]
@module_dirs=<path to directory containing modules>
Greeter=EnglishGreeter
ModuleLoader=LibraryModuleLoader
```

The config file is set up by the SAFE-DART executable normally. By default, the SAFE-DART executable will use `safedart.ini` as the path and `safedart` as the section.The path to the config file, as well as the section within the file to use, can be changed using the `-f` and `-s` arguments respectively.

Programs not using the SAFE-DART executable may set up their own source of configuration data. SAFE-DART using the `Configuration` interface.

### Running the Application
In general, a SAFE-DART application can be run by invoking the SAFE-DART executable as follows:

`<path to SAFE-DART executable> [-f <path to config file>] [-s <config section to use>] <name of implementation of Application to run>`.

For the example above, this could be as simple as:

`safedart GreetApplication`
