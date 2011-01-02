# == initialization ==

AC_INIT(
 [Genetic Functional Programming],
 [0.0.0.10],
 [o01eg@yandex.ru],
 [gfp])

AC_CONFIG_SRCDIR([src/main.cpp])

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror dist-bzip2])

# == basic compiler settings ==

AC_PROG_CXX

AC_PROG_CXX_C_O

AC_PROG_RANLIB

AC_LANG(C++)

AC_TYPE_SIZE_T

CXXFLAGS="-ggdb3 -O0 -Wall -Wextra -pedantic `echo $CXXFLAGS | sed \"s/-O.//\"`"

# == check for libraries ==

AC_ARG_ENABLE([multithread],
	[AS_HELP_STRING([--enable-multithread],
		[support for miltithreading, dependence on gthread @<:@default=no@:>@])],
	[],
	[enable_multithread=no])

AS_IF([test "x$enable_multithread" != xno],
	[],
	[])

GUILE_FLAGS

CXXFLAGS="$GUILE_CFLAGS $CXXFLAGS"
LDFLAGS="$GUILE_LDFLAGS $LDFLAGS"

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
    tools/Makefile
])

AC_OUTPUT

