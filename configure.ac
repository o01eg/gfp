# == initialization ==

AC_INIT(
 [Genetic Functional Programming],
 [0.0.4],
 [o01eg@yandex.ru],
 [gfp])

AC_CONFIG_SRCDIR([src/main.cpp])

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror dist-bzip2 foreign])

# == basic compiler settings ==

AC_PROG_CXX

AC_PROG_CXX_C_O

AC_PROG_RANLIB

AC_LANG(C++)

AC_TYPE_SIZE_T

CXXFLAGS="-Wall -Wextra -Wc++0x-compat -fno-exceptions -fno-rtti -std=c++0x $CXXFLAGS"
LDFLAGS="$LDFLAGS"

AC_ARG_ENABLE([lto],
	[AS_HELP_STRING([--enable-lto], [Enable Link-Time Optimization @<:@default=no@:>@])],
	[CXXFLAGS="-flto $CXXFLAGS";LDFLAGS="-flto $LDFLAGS"],
	[])

AC_ARG_ENABLE([gprof],
	[AS_HELP_STRING([--enable-gprof],
		[support for profiling @<:@default=no@:>@])],
	[CXXFLAGS="-pg $CXXFLAGS";LDFLAGS="-pg $LDFLAGS"],
	[])

AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--enable-debug],
		[support for debugging @<:@default=no@:>@])],
	[CXXFLAGS="-fno-inline -ggdb3 -O0 `echo $CXXFLAGS | sed \"s/-O.//\"`";LGFLAGS="-O0 `echo $LDFLAGS | sed \"s/-O.//\"`"],
	[enable_debug=no])
AM_CONDITIONAL(DEBUG, test "x$enable_debug" != xno)

# == check for libraries ==

AC_ARG_ENABLE([multithread],
	[AS_HELP_STRING([--enable-multithread],
		[support for miltithreading, dependence on pthread @<:@default=no@:>@])],
	[],
	[enable_multithread=no])

AS_IF([test "x$enable_multithread" != xno],
	[AX_PTHREAD([LIBS="$PTHREAD_LIBS $LIBS";CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"], [AC_MSG_ERROR([Sorry, but you need pthread for multithread support.])])],
	[])
AM_CONDITIONAL(MULTITHREAD, test "x$enable_multithread" != xno)

# == check for header files ==

# == check for typedefs, structers and compiler characteristics ==

# == check for library functions ==

# == generate files ==

AC_CONFIG_FILES([
    Makefile
    data/Makefile
    doc/Makefile
    m4/Makefile
    src/Makefile
    src/libfunctions/Makefile
    src/vm/Makefile
    tools/Makefile
	win32/Makefile
])

AC_OUTPUT

echo "CXX: $CXX"
echo "CXXFLAGS: $CXXFLAGS"
echo "LDFLAGS: $LDFLAGS"

