#!/bin/sh
find -iname .*.swp | xargs rm
gvim NEWS README ChangeLog src/*.h src/*.cpp src/vm/*.cpp src/vm/*.h

