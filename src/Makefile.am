SUBDIRS = libfunctions ruby
bin_PROGRAMS = gfp
gfp_SOURCES = conf.cpp \
	      environment.cpp \
	      errors.cpp \
	      ga.cpp \
	      heap.cpp \
	      individual.cpp \
	      ioobject.cpp \
	      jobthread.cpp \
	      logbuf.cpp \
	      main.cpp \
	      object.cpp \
	      program.cpp \
	      stack.cpp
noinst_HEADERS = conf.h \
		 environment.h \
		 errors.h \
		 ga.h \
		 heap.h \
		 individual.h \
		 ioobject.h \
		 jobthread.h \
		 logbuf.h \
		 logger.h \
		 object.h \
		 program.h \
		 stack.h
gfp_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\"
gfp_LDADD = ruby/librubypp.a

