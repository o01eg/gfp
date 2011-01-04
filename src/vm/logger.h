/*
 * Copyright (C) 2010-2011 O01eg <o01eg@yandex.ru> 
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

#include <string>
#include <stdexcept>
#include <sstream>

/* nice compile-time debug strings */
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

#define G__FILE__ static_cast<const char*>(__FILE__)

#define G__FUNCTION__ static_cast<const char*>(__func__)

#define THROW(a) THROW_impl_((a), G__FUNCTION__, G__FILE__, __LINE__)

void inline THROW_impl_(const std::string& a, const char* func, const char* file, unsigned int line)
{
	std::stringstream ss;
	ss << "Exception: " << a << " in " << func << " at " << file << ":" << line;
	throw std::runtime_error(ss.str());
}

void inline THROW_impl_(const char *a, const char* func, const char* file, unsigned int line)
{
	std::stringstream ss;
	ss << "Exception: " << a << " in " << func << " at " << file << ":" << line;
	throw std::runtime_error(ss.str());
}

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7>
std::string inline FormatString(const T1& a1, const T2& a2, const T3& a3, const T4& a4, const T5& a5, const T6& a6, const T7& a7)
{
	std::stringstream ss;
	ss << a1 << a2 << a3 << a4 << a5 << a6 << a7;
	return ss.str();
}

template<class T1, class T2, class T3, class T4, class T5, class T6>
std::string inline FormatString(const T1& a1, const T2& a2, const T3& a3, const T4& a4, const T5& a5, const T6& a6)
{
	std::stringstream ss;
	ss << a1 << a2 << a3 << a4 << a5 << a6;
	return ss.str();
}

template<class T1, class T2, class T3, class T4, class T5>
std::string inline FormatString(const T1& a1, const T2& a2, const T3& a3, const T4& a4, const T5& a5)
{
	std::stringstream ss;
	ss << a1 << a2 << a3 << a4 << a5;
	return ss.str();
}

template<class T1, class T2, class T3, class T4>
std::string inline FormatString(const T1& a1, const T2& a2, const T3& a3, const T4& a4)
{
	std::stringstream ss;
	ss << a1 << a2 << a3 << a4;
	return ss.str();
}

template<class T1, class T2, class T3>
std::string inline FormatString(const T1& a1, const T2& a2, const T3& a3)
{
	std::stringstream ss;
	ss << a1 << a2 << a3;
	return ss.str();
}

template<class T1, class T2>
std::string inline FormatString(const T1& a1, const T2& a2)
{
	std::stringstream ss;
	ss << a1 << a2;
	return ss.str();
}

template<class T1>
std::string inline FormatString(const T1& a1)
{
	std::stringstream ss;
	ss << a1;
	return ss.str();
}

#endif

