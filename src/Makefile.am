SUBDIRS = libfunctions \
	vm
bin_PROGRAMS = gfp gplisp gfpstatic gpoptimize
gfp_SOURCES = conf.cpp \
	current_state.cpp \
	ga.cpp \
	ga_utils.cpp \
	individual.cpp \
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
gfpstatic_SOURCES = gfpstatic.cpp
gpoptimize_SOURCES = conf.cpp \
	ga_utils.cpp \
	gpoptimize.cpp \
	libfunctions/functions.cpp
noinst_HEADERS = conf.h \
	current_state.h \
	dot_dump.h \
	ga.h \
	ga_utils.h \
	individual.h \
	logbuf.h \
	object_utils.h \
	world.h \
	world_file.h

gfp_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\"
gplisp_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\"
gfpstatic_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\" -I.
gpoptimize_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\"

gfpstatic_LDADD = vm/libvm.a
gfp_LDADD = vm/libvm.a
gplisp_LDADD = vm/libvm.a
gpoptimize_LDADD = vm/libvm.a

