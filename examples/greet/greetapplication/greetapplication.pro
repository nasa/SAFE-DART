###################################################################### ##
##
## Developed for NASA Glenn Research Center
## By: Flight Software Branch (LSS)
##
## Project: Software Architecture Framework for Extensibility using Dynamic Assembly of Reflective Types (SAFE-DART
##
## Filename: greetapplication.pro
## File Date: 20171201
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

QT -= gui
TEMPLATE = lib

DESTDIR = $$PWD/../bin
MOC_DIR = $$PWD/moc
OBJECTS_DIR = $$PWD/obj

QMAKE_CXXFLAGS += -std=c++11

SOURCES += greetapplication.cpp module.cpp
HEADERS += greetapplication.h

INCLUDEPATH += \
    $$PWD/../common \
    $$PWD/../../../libsafedart

LIBS += -L$$PWD/../../../bin -lsafedart
