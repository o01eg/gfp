/*
 * Copyright (C) 2010-2012 O01eg <o01eg@yandex.ru> 
 *
 * This file is part of Genetic Function Programming.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <stdexcept>
#include <sstream>
#include <iostream>

/* nice compile-time debug strings */
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

#define G__FILE__ static_cast<const char*>(__FILE__)

#define G__FUNCTION__ static_cast<const char*>(__FUNCTION__)

#define THROW(a) THROW_impl_((a), G__FUNCTION__, G__FILE__, __LINE__)

void inline THROW_impl_(const std::string& a, const char* func, const char* file, unsigned int line)
{
	std::cerr << "Exception: " << a << " in " << func << " at " << file << ":" << line << std::endl;
	abort();
	//throw std::runtime_error(ss.str());
}

void inline THROW_impl_(const char *a, const char* func, const char* file, unsigned int line)
{
	std::cerr << "Exception: " << a << " in " << func << " at " << file << ":" << line << std::endl;
	abort();
	//throw std::runtime_error(ss.str());
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

