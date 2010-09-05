/*
 * Copyright (C) 2010 O01eg <o01eg@yandex.ru> 
 *
 *  This file is part of Genetic Function Programming.
 *
 *  Genetic Function Programming is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Genetic Function Programming is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Genetic Function Programming.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <glibmm/ustring.h>
#include <glibmm/error.h>

/* nice compile-time debug strings */
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

#define G__FILE__ static_cast<const char*>(__FILE__)

#ifdef _WIN32
#define G__FUNCTION__ static_cast<const char*>(__FUNCTION__)
#else
#define G__FUNCTION__ static_cast<const char*>(__func__)
#endif

#define THROW(a) THROW_impl_((a), G__FUNCTION__, G__FILE__, __LINE__)

#ifdef _WIN32
void inline THROW_impl_(const Glib::ustring& a, const char* func, const char* file, unsigned int line)
{
	throw Glib::Error(1, 0, Glib::ustring::compose("Exception: %1 in %2 at %3:%4", a, func, file, line));
}
#else
template <class T> void inline THROW_impl_(const T& a, const char* func, const char* file, unsigned int line)
{
	throw Glib::Error(1, 0, Glib::ustring::compose("Exception: %1 in %2 at %3:%4", a, func, file, line));
}

template <int N> void inline THROW_impl_(const char a[N], const char* func, const char* file, unsigned int line)
{
	throw Glib::Error(1, 0, Glib::ustring::compose("Exception: %1 in %2 at %3:%4", static_cast<const char*>(a), func, file, line));
}
#endif

#endif

