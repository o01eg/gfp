SUBDIRS = libfunctions ruby
bin_PROGRAMS = gfp gplisp gptest
gfp_SOURCES = environment.cpp \
	ga.cpp \
	ga_utils.cpp \
	heap.cpp \
	individual.cpp \
	ioobject.cpp \
	jobthread.cpp \
	logbuf.cpp \
	main.cpp \
	object.cpp \
	program.cpp 
gplisp_SOURCES = environment.cpp \
	gplisp.cpp \
	heap.cpp \
	ioobject.cpp \
	logbuf.cpp \
	object.cpp \
	program.cpp
gptest_SOURCES = dot_dump.cpp \
	environment.cpp \
	ga_utils.cpp \
	gptest.cpp \
	heap.cpp \
	ioobject.cpp \
	logbuf.cpp \
	object.cpp \
	program.cpp
noinst_HEADERS = conf.h \
	dot_dump.h \
	environment.h \
	errors.h \
	ga.h \
	ga_utils.h \
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
gplisp_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\"
gptest_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\" -D_DEBUG_ENV_ -D_DEBUG_HEAP_ -D_DEBUG_OBJECT_ -D_DEBUG_EVAL_ -D_DOT_MEMORY_

