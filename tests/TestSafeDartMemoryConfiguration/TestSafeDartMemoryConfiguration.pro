###################################################################### ##
##
## Developed for NASA Glenn Research Center
## By: Flight Software Branch (LSS)
##
## Project: Flow Boiling and Condensation Experiment (FBCE)
## Candidate for GOTS reuse once FBCE has completed V&V testing
##
## Filename: TestSafeDartMemoryConfiguration.pro
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

QT       += testlib
QT       -= gui

TARGET = tst_testsafedartmemoryconfiguration
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += test

TEMPLATE = app

DEFINES += SRCDIR=\\\"$$PWD/\\\"
SOURCES += \
    $$PWD/tst_testsafedartmemoryconfiguration.cpp

QMAKE_CXXFLAGS += --std=c++11

QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0
LIBS += \
    -lgcov

INCLUDEPATH += $$PWD/../SafeDartUtil
INCLUDEPATH += $$PWD/../../libsafedart

include($$PWD/../SafeDartUtil/SafeDartUtil.pro)
include($$PWD/../../libsafedart/libsafedart.pro)
