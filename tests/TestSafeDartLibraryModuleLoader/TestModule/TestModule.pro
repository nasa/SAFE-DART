###################################################################### ##
##
## Developed for NASA Glenn Research Center
## By: Flight Software Branch (LSS)
##
## Project: Flow Boiling and Condensation Experiment (FBCE)
##
## Filename: TestModule.pro
## File Date: 20170811
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
###################################################################### ##

QT       += core
QT       -= gui

TARGET = testmodule
TEMPLATE  = lib

INCLUDEPATH += \
    $$PWD/../../../libsafedart

SOURCES += \
    $$PWD/module.cpp

QMAKE_CXXFLAGS += -std=c++11

