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
## Copyright © 2017 United States Government as represented by NASA Glenn Research Center.
## No copyright is claimed in the United States under Title 17, U.S.Code. All Other Rights Reserved.
## See LICENSE.txt in the root of the repository for more details.
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
