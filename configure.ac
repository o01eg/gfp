# == initialization ==

AC_INIT(
 [Genetic Functional Programming],
 [0.0.7],
 [o01eg@yandex.ru],
 [gfp])

AC_CONFIG_SRCDIR([src/gfp-labirinth-threaded.cpp])

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror dist-bzip2 foreign])

# == basic compiler settings ==

AC_PROG_CXX

AC_PROG_CXX_C_O

AC_PROG_RANLIB

AC_LANG(C++)

AC_TYPE_SIZE_T

CXXFLAGS="-Wall -Wextra -fno-exceptions -fno-rtti $CXXFLAGS"
LDFLAGS="$LDFLAGS"

AX_CHECK_COMPILE_FLAG([-std=c++11],[CXXFLAGS="-std=c++11 -Wc++11-compat $CXXFLAGS"],[
	AX_CHECK_COMPILE_FLAG([-std=c++0x],[CXXFLAGS="-std=c++0x -Wc++0x-compat $CXXFLAGS"],[AC_MSG_ERROR([Compiler not support C++11])])
])

AC_ARG_ENABLE([lto],
	[AS_HELP_STRING([--enable-lto], [Enable Link-Time Optimization @<:@default=no@:>@])],
	[CXXFLAGS="-flto $CXXFLAGS";LDFLAGS="-flto -fuse-linker-plugin $LDFLAGS"],
	[])

AC_ARG_ENABLE([gprof],
	[AS_HELP_STRING([--enable-gprof],
		[support for profiling @<:@default=no@:>@])],
	[CXXFLAGS="-pg $CXXFLAGS";LDFLAGS="-pg $LDFLAGS"],
	[])

AC_ARG_ENABLE([prof-gen],
	[AS_HELP_STRING([--enable-prof-gen],
		[generate profile information @<:@default=no@:>@])],
	[CXXFLAGS="-fprofile-generate $CXXFLAGS";LDFLAGS="-fprofile-generate $LDFLAGS"],
	[])

AC_ARG_ENABLE([prof-use],
	[AS_HELP_STRING([--enable-prof-use],
		[use profile information @<:@default=no@:>@])],
	[CXXFLAGS="-fprofile-use -fprofile-correction $CXXFLAGS";LDFLAGS="-fprofile-use -fprofile-correction $LDFLAGS"],
	[])

AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--enable-debug],
		[support for debugging @<:@default=no@:>@])],
	[CXXFLAGS="-fno-inline -ggdb3 -O0 `echo $CXXFLAGS | sed \"s/-O.//\"`"
	LGFLAGS="-O0 `echo $LDFLAGS | sed \"s/-O.//\"`"
	AC_DEFINE([_DEBUG_OBJECT_],[1],[Desc])
	AC_DEFINE([_DEBUG_HEAP_],[1],[Desc])
#	AC_DEFINE([_DEBUG_EVAL_],[1],[Desc])
	AC_DEFINE([_DOT_MEMORY_],[1],[Desc])],
	[enable_debug=no])
AM_CONDITIONAL(DEBUG, test "x$enable_debug" != xno)

# == check for libraries ==

AC_LANG_PUSH(C++)

# == check for header files ==

AC_CHECK_HEADERS([unordered_map], [], [AC_MSG_ERROR([Header <unordered_map> not found])])
AC_CHECK_HEADERS([thread], [], [AC_MSG_ERROR([Header <thread> not found])])

# == check for typedefs, structers and compiler characteristics ==

# == check for library functions ==

AX_PTHREAD([LDFLAGS="$PTHREAD_LIBS $LDFLAGS";CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"])

# == generate files ==

AC_LANG_POP(C++)

AC_CONFIG_FILES([
	Makefile
	data/Makefile
	doc/Makefile
	m4/Makefile
	src/Makefile
	src/vm/Makefile
	tools/Makefile
	win32/Makefile
])

AC_OUTPUT

echo "CXX: $CXX"
echo "CXXFLAGS: $CXXFLAGS"
echo "LDFLAGS: $LDFLAGS"

