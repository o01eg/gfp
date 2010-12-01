SUBDIRS = libfunctions ruby
bin_PROGRAMS = gfp gplisp gptest
gfp_SOURCES = current_state.cpp \
	environment.cpp \
	ga.cpp \
	ga_utils.cpp \
	heap.cpp \
	individual.cpp \
	ioobject.cpp \
	jobthread.cpp \
	logbuf.cpp \
	main.cpp \
	object.cpp \
	object_utils.cpp \
	program.cpp \
	weak_object.cpp \
	world.cpp \
	world_file.cpp
gplisp_SOURCES = environment.cpp \
	gplisp.cpp \
	heap.cpp \
	ioobject.cpp \
	logbuf.cpp \
	object.cpp \
	program.cpp \
	weak_object.cpp
gptest_SOURCES = dot_dump.cpp \
	environment.cpp \
	ga_utils.cpp \
	gptest.cpp \
	heap.cpp \
	ioobject.cpp \
	logbuf.cpp \
	object.cpp \
	program.cpp \
	weak_object.cpp
noinst_HEADERS = conf.h \
	current_state.h \
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
	object_utils.h \
	program.h \
	stack.h \
	weak_object.h \
	world.h \
	world_file.h
gfp_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\"
if COMPILE_RUBY
gfp_LDADD = ruby/librubypp.a
endif
gplisp_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\" -D_DEBUG_EVAL_
gptest_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\" -D_DEBUG_ENV_ -D_DEBUG_HEAP_ -D_DEBUG_OBJECT_ -D_DEBUG_EVAL_ -D_DOT_MEMORY_

