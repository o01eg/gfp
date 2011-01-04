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

#ifndef _SCHEME_CATCH_H_
#define _SCHEME_CATCH_H_

#include <libguile.h>

SCM CatchHandle(void *data, SCM key, SCM parameters);

template<class R>
class CatchPtr0
{
public:
	typedef R (*T_func)();
	CatchPtr0(const T_func& func)
		:m_Func(func)
	{	
	}
	~CatchPtr0()
	{
	}
	static R Body(void *data)
	{
		CatchPtr0 *self = reinterpret_cast<CatchPtr0*>(data);
		return static_cast<R>(self->m_Func());
	}
	R operator()()
	{
		return scm_c_catch(SCM_BOOL_T, Body, reinterpret_cast<void*>(this), CatchHandle, NULL, NULL, NULL);
	}
private:
	T_func m_Func;
};

template<class R, class A1>
class CatchPtr1
{
public:
	typedef R (*T_func)(A1);
	CatchPtr1(const T_func& func, const A1& a1)
		:m_Func(func), m_A1(a1)
	{	
	}
	~CatchPtr1()
	{
	}
	static R Body(void *data)
	{
		CatchPtr1 *self = reinterpret_cast<CatchPtr1*>(data);
		return static_cast<R>(self->m_Func(self->m_A1));
	}
	R operator()()
	{
		return scm_c_catch(SCM_BOOL_T, Body, reinterpret_cast<void*>(this), CatchHandle, NULL, NULL, NULL);
	}
private:
	T_func m_Func;
	A1 m_A1;
};

template<class R>
R Catch(R (*func)())
{
	CatchPtr0<R> c = CatchPtr0<R>(func);
	return c();
}

template<class R, class A1>
R Catch(R (*func)(A1), A1 a1)
{
	CatchPtr1<R, A1> c = CatchPtr1<R, A1>(func, a1);
	return c();
}

#endif

