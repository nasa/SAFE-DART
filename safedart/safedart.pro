###################################################################### ##
##
## Developed for NASA Glenn Research Center
## By: Flight Software Branch (LSS)
##
## Project: Flow Boiling and Condensation Experiment (FBCE)
## Candidate for GOTS reuse once FBCE has completed V&V testing
##
## Filename: safedart.pro
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
    QT       += core
    QT       -= gui

    TARGET = safedart
    CONFIG   += console
    CONFIG   -= app_bundle

    TEMPLATE = app

    SOURCES += $$PWD/main.cpp

    QMAKE_CXXFLAGS += --std=c++11

    unix|win32: LIBS += -L$$PWD/../bin -lsafedart

    DESTDIR = $$PWD/../bin
    MOC_DIR = $$PWD/moc
    OBJECTS_DIR = $$PWD/obj
}

HEADERS += \
    $$PWD/doxygen.h \
    $$PWD/safeapplication.h \
    $$PWD/safeconfiguration.h

SOURCES += \
    $$PWD/safeapplication.cpp \
    $$PWD/safeconfiguration.cpp

INCLUDEPATH += $$PWD/../libsafedart
