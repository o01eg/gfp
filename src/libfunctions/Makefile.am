bin_NOINST_PROGRAMS = libfunctions.so
bin_NOINSTdir = $(pkglibdir)
libfunctions_so_SOURCES = functions.cpp # ../object.cpp ../environment.cpp ../heap.cpp
libfunctions_so_CXXFLAGS = -I.. -fPIC
libfunctions_so_LDFLAGS = -shared -Wl,-soname,libfunctions.so.0 -Wl,-export-dynamic
#noinst_HEADERS = functions.h

