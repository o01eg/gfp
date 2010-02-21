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

#define G__FUNCTION__ static_cast<const char*>(__FUNCTION__)
#define G__FILE__ static_cast<const char*>(__FILE__)
#define THROW(a) throw Glib::Error(1, 0, Glib::ustring::compose("Exception: %1 in %2 at %3:%4",\
	(a), G__FUNCTION__, G__FILE__, __LINE__))

#endif

