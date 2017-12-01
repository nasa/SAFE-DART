###################################################################### ##
##
## Developed for NASA Glenn Research Center
## By: Flight Software Branch (LSS)
##
## Project: Flow Boiling and Condensation Experiment (FBCE)
## Candidate for GOTS reuse once FBCE has completed V&V testing
##
## Filename: SafeDartUtil.pro
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
###################################################################### ##


HEADERS += \
    $$PWD/messagetracker.h \
    $$PWD/mockapplication.h \
    $$PWD/mockmoduleloader.h \
    $$PWD/openbuilder.h

SOURCES += \
    $$PWD/messagetracker.cpp \
    $$PWD/mockapplication.cpp \
    $$PWD/mockmoduleloader.cpp \
    $$PWD/openbuilder.cpp

INCLUDEPATH += $$PWD/../../libsafedart
