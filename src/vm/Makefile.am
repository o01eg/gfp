noinst_HEADERS = environment.h \
	heap.h \
	ioobject.h \
	logger.h \
	object.h \
	program.h \
	stack.h \
	symbols.h \
	weak_object.h

if DEBUG
noinst_LIBRARIES = libvm.a libvm_d.a
else
noinst_LIBRARIES = libvm.a
endif

libvm_a_SOURCES = environment.cpp \
	heap.cpp \
	ioobject.cpp \
	object.cpp \
	program.cpp \
	weak_object.cpp

if DEBUG
libvm_d_a_SOURCES = $(libvm_a_SOURCES)

libvm_d_a_CPPFLAGS = -D_DEBUG_OBJECT_ -D_DEBUG_HEAP_ -D_DEBUG_EVAL_ -D_DEBUG_HEAP_ -D_DOT_MEMORY_
endif

