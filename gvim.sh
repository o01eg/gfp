#!/bin/sh
find -iname .*.swp | xargs rm
gvim NEWS README ChangeLog src/*.h src/*.cpp src/libfunctions/*.cpp src/ruby/*.cpp src/ruby/*.h
