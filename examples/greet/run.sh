#!/bin/sh
qmake greet.pro && make && LD_LIBRARY_PATH=../../bin ../../bin/safedart GreetApplication
