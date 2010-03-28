#ifndef _SHARED_H_
#define _SHARED_H_

#ifdef HAVE_CONFIG_H
	#include <config.h>
	#ifdef HAVE_SYS_TYPES_H
		#include <sys/types.h>
	#endif
#else
#include <cstdlib>
#endif

#include <glibmm/ustring.h>
#include <glibmm/error.h>

/* nice compile-time debug strings */
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

#define G__FUNCTION__ static_cast<const char*>(__func__)
#define G__FILE__ static_cast<const char*>(__FILE__)
#define THROW(a) THROW_impl_((a), G__FUNCTION__, G__FILE__, __LINE__)

template <class T> void inline THROW_impl_(T a, const char* func, const char* file, unsigned int line)
{
	Glib::Error(1, 0, Glib::ustring::compose("Exception: %1 in %2 at %3:%4",
		a, func, file, line));
}

template <int N> void inline THROW_impl_(const char a[N], const char* func, const char* file, unsigned int line)
{
	Glib::Error(1, 0, Glib::ustring::compose("Exception: %1 in %2 at %3:%4",
		static_cast<const char*>(a), func, file, line));
}

#endif

