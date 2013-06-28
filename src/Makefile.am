SUBDIRS = vm
bin_PROGRAMS = gfp-labirinth gplisp gfpstatic gpoptimize
gfp_labirinth_SOURCES = conf.cpp \
	current_state.cpp \
	functions.cpp \
	ga.cpp \
	ga_utils.cpp \
	individual.cpp \
	logbuf.cpp \
	main.cpp \
	object_utils.cpp \
	thread-pool.cpp \
	world.cpp \
	world_file.cpp
gplisp_SOURCES = current_state.cpp \
	functions.cpp \
	gplisp.cpp \
	logbuf.cpp
gfpstatic_SOURCES = gfpstatic.cpp
gpoptimize_SOURCES = conf.cpp \
	functions.cpp \
	ga_utils.cpp \
	gpoptimize.cpp
noinst_HEADERS = conf.h \
	current_state.h \
	dot_dump.h \
	functions.h \
	ga.h \
	ga_utils.h \
	individual.h \
	logbuf.h \
	object_utils.h \
	thread-pool.h \
	world.h \
	world_file.h

gfp_labirinth_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\"
gplisp_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\"
gfpstatic_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\" -I.
gpoptimize_CPPFLAGS = -DDATA_DIR=\"$(pkgdatadir)/\" -DLIB_DIR=\"$(pkglibdir)/\"

gfpstatic_LDADD = vm/libvm.a
gfp_labirinth_LDADD = vm/libvm.a
gplisp_LDADD = vm/libvm.a
gpoptimize_LDADD = vm/libvm.a

