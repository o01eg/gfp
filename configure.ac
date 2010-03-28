# == initialization ==

AC_INIT(
 [Genetic Functional Programming],
 [0.0.0.1],
 [o01eg@yandex.ru],
 [gfp])

AC_CONFIG_SRCDIR([src/main.cpp])

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror dist-bzip2])

ruby_binary="ruby"
AC_ARG_WITH(ruby,
 [  --with-ruby     Path to ruby interpreter.], 
 [if test $withval == "no"
  then
   ruby_binary="ruby"
  else
   ruby_binary=$withval
  fi],
 ruby_binary="ruby"
)

# == basic compiler settings ==

AC_PROG_CXX

AC_PROG_CXX_C_O

#AC_PROG_RANLIB

AC_LANG(C++)

AC_TYPE_SIZE_T

CXXFLAGS="-ggdb3 -Wall -Wextra -pedantic -D_DEBUG_ENV_ -D_DEBUG_HEAP_ -D_DEBUG_OBJECT_ -D_DEBUG_EVAL_"

# == check for libraries ==

PKG_CHECK_MODULES([GTHREAD],[gthread-2.0],[LDFLAGS="$GTHREAD_LIBS $LDFLAGS";CXXFLAGS="$GTHREAD_CFLAGS $CXXFLAGS"],[AC_MSG_ERROR([Sorry, but you need GThread library])])
PKG_CHECK_MODULES([GIOMM],[giomm-2.4],[LDFLAGS="$GIOMM_LIBS $LDFLAGS";CXXFLAGS="$GIOMM_CFLAGS $CXXFLAGS"],[AC_MSG_ERROR([Sorry, but you need GLibmm library])])
PKG_CHECK_MODULES([GLIBMM],[glibmm-2.4],[LDFLAGS="$GLIBMM_LIBS $LDFLAGS";CXXFLAGS="$GLIBMM_CFLAGS $CXXFLAGS"],[AC_MSG_ERROR([Sorry, but you need GLibmm library])])

HAVE_RUBY=1
if test $ruby_binary == "ruby"
then
 AC_PATH_PROG([RUBY], [ruby], [AC_MSG_RESULT([no])])
else
 AC_CHECK_FILE([$ruby_binary], RUBY=$ruby_binary, [AC_MSG_ERROR([Sorry, but you need Ruby])])
fi

if test $RUBY != "no"
then
 RUBY_LDFLAGS=[`$RUBY -e 'require "rbconfig"; print Config::CONFIG["LIBRUBYARG_SHARED"], "\n"'`]
 AC_SUBST(RUBY_LDFLAGS)
 RUBY_CXXFLAGS=[-I`$RUBY -e 'require "rbconfig"; print Config::CONFIG["rubyhdrdir"], "\n"'`]
 AC_SUBST(RUBY_CXXFLAGS)

 LDFLAGS="$RUBY_LDFLAGS $LDFLAGS"
 CXXFLAGS="$RUBY_CXXFLAGS $CXXFLAGS"

 echo -ne "Checking for Ruby C API... "
 AC_LINK_IFELSE(AC_LANG_PROGRAM,[echo "yes"],[AC_MSG_ERROR([Sorry, but you need Ruby C API])])
 AC_DEFINE([HAVE_RUBY], [1], [Set if program compiled with Ruby support])
fi

# == check for header files ==

# == check for typedefs, structers and compiler characteristics ==

# == check for library functions ==

# == generate files ==

AC_CONFIG_FILES([
    Makefile
    data/Makefile
    doc/Makefile
    src/Makefile
    src/libfunctions/Makefile
    win32/Makefile
])

AC_OUTPUT

