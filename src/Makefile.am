SUBDIRS = libfunctions \
	scheme \
	vm
bin_PROGRAMS = gfp gplisp
gfp_SOURCES = conf.cpp \
	current_state.cpp \
	ga.cpp \
	ga_utils.cpp \
	individual.cpp \
	jobthread.cpp \
	libfunctions/functions.cpp \
	logbuf.cpp \
	main.cpp \
	object_utils.cpp \
	world.cpp \
	world_file.cpp
gplisp_SOURCES = current_state.cpp \
	gplisp.cpp \
	libfunctions/functions.cpp \
	logbuf.cpp
noinst_HEADERS = conf.h \
	current_state.h \
	dot_dump.h \
	errors.h \
	ga.h \
	ga_utils.h \
	individual.h \
	jobthread.h \
	logbuf.h \
	object_utils.h \
	world.h \
	world_file.h

gfp_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\"
gplisp_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\"

gfp_LDADD = scheme/libscheme.a \
	vm/libvm.a

gplisp_LDADD = vm/libvm_d.a

