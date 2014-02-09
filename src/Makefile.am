SUBDIRS = vm
bin_PROGRAMS = gfp-labirinth-threaded gplisp gfpstatic gpoptimize
gfp_labirinth_threaded_SOURCES = conf.cpp \
	functions.cpp \
	ga.cpp \
	ga_utils.cpp \
	gfp-labirinth-threaded.cpp \
	individual.cpp \
	logbuf.cpp \
	object_utils.cpp \
	world.cpp \
	world_file.cpp
gplisp_SOURCES = functions.cpp \
	gplisp.cpp \
	logbuf.cpp
gfpstatic_SOURCES = gfpstatic.cpp
gpoptimize_SOURCES = conf.cpp \
	functions.cpp \
	ga_utils.cpp \
	gpoptimize.cpp
noinst_HEADERS = conf.h \
	dot_dump.h \
	functions.h \
	ga.h \
	ga_utils.h \
	individual.h \
	logbuf.h \
	object_utils.h \
	world.h \
	world_file.h

gfp_labirinth_threaded_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\"
gplisp_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\"
gfpstatic_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\" -I.
gpoptimize_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\"

gfpstatic_LDADD = vm/libvm.a
gfp_labirinth_threaded_LDADD = vm/libvm.a
gplisp_LDADD = vm/libvm.a
gpoptimize_LDADD = vm/libvm.a

