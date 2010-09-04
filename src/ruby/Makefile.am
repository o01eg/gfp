noinst_HEADERS = error.h \
	protect.h \
	r_string.h \
	r_symbol.h \
	rubypp.h \
	value.h
if COMPILE_RUBY
noinst_LIBRARIES = librubypp.a

librubypp_a_SOURCES = error.cpp \
	rubypp.cpp
else
EXTRA_DIST = error.cpp \
	rubypp.cpp
endif

