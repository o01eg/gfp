#!/bin/sh
find -iname .*.swp | xargs rm
gvim NEWS README ChangeLog src/*.h src/*.cpp src/gpstatic/*.cpp src/gpstatic/*.h src/libfunctions/*.cpp src/libfunctions/*.h src/scheme/*.h src/scheme/*.cpp src/vm/*.cpp src/vm/*.h

