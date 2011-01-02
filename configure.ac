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
	[PKG_CHECK_MODULES([GTHREAD],[gthread-2.0],[LDFLAGS="$GTHREAD_LIBS $LDFLAGS";CXXFLAGS="$GTHREAD_CFLAGS $CXXFLAGS"],[AC_MSG_ERROR([Sorry, but you need GThread library])])
	],
	[])

PKG_CHECK_MODULES([GLIBMM],[glibmm-2.4],[LDFLAGS="$GLIBMM_LIBS $LDFLAGS";CXXFLAGS="$GLIBMM_CFLAGS $CXXFLAGS"],[AC_MSG_ERROR([Sorry, but you need GLibmm library])])

AC_ARG_ENABLE([static],
	[AS_HELP_STRING([--enable-static],
		[support for static compiled functions @<:@default=yes@:>@])],
	[],
	[enable_static=yes])
AM_CONDITIONAL(COMPILE_STATIC, test "x$enable_static" != xno)

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
    win32/Makefile
])

AC_OUTPUT

