bin_NOINST_PROGRAMS = libfunctions.so
bin_NOINSTdir = $(pkglibdir)
libfunctions_so_SOURCES = functions.cpp ../object.cpp ../heap.cpp ../weak_object.cpp ../environment.cpp
libfunctions_so_CXXFLAGS = -I.. -fPIC -DLIB_DIR=\"$(pkglibdir)/\"
libfunctions_so_LDFLAGS = -shared -Wl,-soname,libfunctions.so.0 -Wl,-export-dynamic
#noinst_HEADERS = functions.h

