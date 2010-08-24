SUBDIRS = libfunctions ruby
bin_PROGRAMS = gfp gplisp gptest
gfp_SOURCES = conf.cpp \
	environment.cpp \
	errors.cpp \
	ga.cpp \
	ga_utils.cpp \
	heap.cpp \
	individual.cpp \
	ioobject.cpp \
	jobthread.cpp \
	logbuf.cpp \
	main.cpp \
	object.cpp \
	program.cpp \
	stack.cpp
gplisp_SOURCES = environment.cpp \
	errors.cpp \
	gplisp.cpp \
	heap.cpp \
	ioobject.cpp \
	logbuf.cpp \
	object.cpp \
	program.cpp \
	stack.cpp
gptest_SOURCES = dot_dump.cpp \
	environment.cpp \
	errors.cpp \
	ga_utils.cpp \
	gptest.cpp \
	heap.cpp \
	ioobject.cpp \
	logbuf.cpp \
	object.cpp \
	program.cpp \
	stack.cpp
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
gptest_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\"

