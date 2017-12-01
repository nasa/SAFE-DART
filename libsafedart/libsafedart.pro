###################################################################### ##
##
## Developed for NASA Glenn Research Center
## By: Flight Software Branch (LSS)
##
## Project: Flow Boiling and Condensation Experiment (FBCE)
## Candidate for GOTS reuse once FBCE has completed V&V testing
##
## Filename: libsafedart.pro
## File Date: 20160928
##
## Authors ##
## Author: Jordan R. Higgins (JRH)
##
## Version and Traceability ##
## Subversion: @version $Id$
##
## Revision History:
##   <Date> <Name of Change Agent>
##   Description:
##     - Bulleted list of changes.
##
## Copyright © 2017 United States Government as represented by NASA Glenn Research Center.
## No copyright is claimed in the United States under Title 17, U.S.Code. All Other Rights Reserved.
## See LICENSE.txt in the root of the repository for more details.
##
###################################################################### ##

!test {
    QT       -= gui

    TARGET = safedart
    TEMPLATE = lib

    QMAKE_CXXFLAGS += --std=c++11

    DESTDIR = $$PWD/../bin
    MOC_DIR = $$PWD/moc
    OBJECTS_DIR = $$PWD/obj
}

INCLUDEPATH += \
    $$PWD

HEADERS += \
    $$PWD/application.h \
    $$PWD/builder.h \
    $$PWD/configuration.h \
    $$PWD/doxygen.h \
    $$PWD/librarymoduleloader.h \
    $$PWD/memoryconfiguration.h \
    $$PWD/module.h \
    $$PWD/moduleloader.h \
    $$PWD/reflectable.h \
    $$PWD/settingsconfiguration.h

SOURCES += \
    $$PWD/builder.cpp \
    $$PWD/configuration.cpp \
    $$PWD/librarymoduleloader.cpp \
    $$PWD/memoryconfiguration.cpp \
    $$PWD/settingsconfiguration.cpp
