# == initialization ==

AC_INIT(
    [Genetic Functional Programming],
    [0.0.0.1],
    [o01eg@yandex.ru],
    [gfp])

AC_CONFIG_SRCDIR([src/main.cpp])

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror dist-bzip2])

# == basic compiler settings ==

AC_PROG_CXX

AC_PROG_CXX_C_O

#AC_PROG_RANLIB

AC_LANG(C++)

CXXFLAGS="-ggdb3 -Wall -Wextra -pedantic -D_DEBUG_ENV_ -D_DEBUG_HEAP_ -D_DEBUG_OBJECT_"

# == check for libraries ==

PKG_CHECK_MODULES([GTHREAD],[gthread-2.0],[LDFLAGS="$GTHREAD_LIBS $LDFLAGS";CXXFLAGS="$GTHREAD_CFLAGS $CXXFLAGS"],[AC_MSG_ERROR([Sorry, but you need GThread library])])
PKG_CHECK_MODULES([GIOMM],[giomm-2.4],[LDFLAGS="$GIOMM_LIBS $LDFLAGS";CXXFLAGS="$GIOMM_CFLAGS $CXXFLAGS"],[AC_MSG_ERROR([Sorry, but you need GLibmm library])])
PKG_CHECK_MODULES([GLIBMM],[glibmm-2.4],[LDFLAGS="$GLIBMM_LIBS $LDFLAGS";CXXFLAGS="$GLIBMM_CFLAGS $CXXFLAGS"],[AC_MSG_ERROR([Sorry, but you need GLibmm library])])

# == check for header files ==

# == check for typedefs, structers and compiler characteristics ==

AC_TYPE_SIZE_T

# == check for library functions ==

# == generate files ==

AC_CONFIG_FILES([
    Makefile
    data/Makefile
    doc/Makefile
    src/Makefile
    src/libfunctions/Makefile
])

AC_OUTPUT

