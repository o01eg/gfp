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
	static SCM Body(void *data)
	{
		CatchPtr0 *self = reinterpret_cast<CatchPtr0*>(data);
		self->m_R = self->m_Func();
		return SCM_BOOL_T;
	}
	R operator()()
	{
		scm_c_catch(SCM_BOOL_T, Body, reinterpret_cast<void*>(this), CatchHandle, NULL, NULL, NULL);
		return m_R;
	}
private:
	T_func m_Func;
	R m_R;
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
	static SCM Body(void *data)
	{
		CatchPtr1 *self = reinterpret_cast<CatchPtr1*>(data);
		self->m_R = self->m_Func(self->m_A1);
		return SCM_BOOL_T;
	}
	R operator()()
	{
		scm_c_catch(SCM_BOOL_T, Body, reinterpret_cast<void*>(this), CatchHandle, NULL, NULL, NULL);
		return m_R;
	}
private:
	T_func m_Func;
	A1 m_A1;
	R m_R;
};

template<class A1>
class CatchPtr1<SCM, A1>
{
public:
	typedef SCM (*T_func)(A1);
	CatchPtr1(const T_func& func, const A1& a1)
		:m_Func(func), m_A1(a1)
	{	
	}
	~CatchPtr1()
	{
	}
	static SCM Body(void *data)
	{
		CatchPtr1 *self = reinterpret_cast<CatchPtr1*>(data);
		return self->m_Func(self->m_A1);
	}
	SCM operator()()
	{
		return scm_c_catch(SCM_BOOL_T, Body, reinterpret_cast<void*>(this), CatchHandle, NULL, NULL, NULL);
	}
private:
	T_func m_Func;
	A1 m_A1;
};

template<class R, class A1, class A2>
class CatchPtr2
{
public:
	typedef R (*T_func)(A1, A2);
	CatchPtr2(const T_func& func, const A1& a1, const A2 &a2)
		:m_Func(func), m_A1(a1), m_A2(a2)
	{	
	}
	~CatchPtr2()
	{
	}
	static SCM Body(void *data)
	{
		CatchPtr2 *self = reinterpret_cast<CatchPtr2*>(data);
		self->m_R = self->m_Func(self->m_A1, self->m_A2);
		return SCM_BOOL_T;
	}
	R operator()()
	{
		scm_c_catch(SCM_BOOL_T, Body, reinterpret_cast<void*>(this), CatchHandle, NULL, NULL, NULL);
		return m_R;
	}
private:
	T_func m_Func;
	A1 m_A1;
	A2 m_A2;
	R m_R;
};

template<class R, class A1, class A2, class A3>
class CatchPtr3
{
public:
	typedef R (*T_func)(A1, A2, A3);
	CatchPtr3(const T_func& func, const A1& a1, const A2 &a2, const A3& a3)
		:m_Func(func), m_A1(a1), m_A2(a2), m_A3(a3)
	{	
	}
	~CatchPtr3()
	{
	}
	static SCM Body(void *data)
	{
		CatchPtr3 *self = reinterpret_cast<CatchPtr3*>(data);
		self->m_R = self->m_Func(self->m_A1, self->m_A2, self->m_A3);
		return SCM_BOOL_T;
	}
	R operator()()
	{
		scm_c_catch(SCM_BOOL_T, Body, reinterpret_cast<void*>(this), CatchHandle, NULL, NULL, NULL);
		return m_R;
	}
private:
	T_func m_Func;
	A1 m_A1;
	A2 m_A2;
	A3 m_A3;
	R m_R;
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

template<class R, class A1, class A2>
R Catch(R (*func)(A1, A2), A1 a1, A2 a2)
{
	CatchPtr2<R, A1, A2> c = CatchPtr2<R, A1, A2>(func, a1, a2);
	return c();
}

template<class R, class A1, class A2, class A3>
R Catch(R (*func)(A1, A2, A3), A1 a1, A2 a2, A3 a3)
{
	CatchPtr3<R, A1, A2, A3> c = CatchPtr3<R, A1, A2, A3>(func, a1, a2, a3);
	return c();
}

#endif

