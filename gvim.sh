#!/bin/sh
find -iname .*.swp | xargs rm
gvim NEWS README ChangeLog src/*.h src/*.cpp src/libfunctions/*.cpp src/libfunctions/*.h src/vm/*.cpp src/vm/*.h

