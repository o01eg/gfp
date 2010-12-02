# == initialization ==

AC_INIT(
 [Genetic Functional Programming],
 [0.0.0.7],
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

CXXFLAGS="-ggdb3 -Wall -Wextra -pedantic"

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

# ToDo: delete it when Gentoo move into ruby 1.9
PATH="/mnt/other/games/ruby/bin:$PATH"

AC_ARG_ENABLE([ruby],
	[AS_HELP_STRING([--enable-ruby],
		[support for ruby scripts @<:@default=no@:>@])],
	[],
	[enable_ruby=no])

AS_IF([test "x$enable_ruby" != xno],
	[AM_CHECK_RUBY([1.9])
	AC_SUBST(CXXFLAGS,"$CXXFLAGS $RUBY_CFLAGS")
	AC_SUBST(LDFLAGS,"$LDFLAGS $RUBY_LIBS")
	AC_CHECK_HEADERS([ruby.h],,AC_MSG_ERROR([ruby.h header not found]))
	AC_CHECK_LIB([ruby], [ruby_init],,AC_MSG_ERROR([ruby library not found]))
	],
	[])

AM_CONDITIONAL(COMPILE_RUBY, test "x$enable_ruby" != xno)

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
    src/ruby/Makefile
    tools/Makefile
    win32/Makefile
])

AC_OUTPUT

