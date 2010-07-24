#ifndef _RUBY_PROTECT_HPP_
#define _RUBY_PROTECT_HPP_

#include <stdexcept>
#include "ruby.hpp"
#include "error.hpp"

namespace Ruby
{
	template<class R, class C> class ProtectMem0
	{
	public:
		typedef R (C::*T_func)();
		ProtectMem0(C* obj, T_func func)
			:m_Obj(obj),
			m_Func(func)
		{
		}
		~ProtectMem0()
		{
		}
		static VALUE FuncR(VALUE arg)
		{
			ProtectMem0 *self = (ProtectMem0*)arg;
			R res = (self->m_Obj->*(self->m_Func))();
			return (VALUE)res;
		}
		R operator()()
		{
			int state;
			R res = (R)rb_protect(ProtectMem0<R, C>::FuncR, (VALUE)this, &state);
			if(state != 0)
			{
				throw Error(state);
			}
			return res;
		}
	private:
		C* m_Obj;
		T_func m_Func;
	};

	template<class C> class ProtectMem0<void, C>
	{
	public:
		typedef void (C::*T_func)();
		ProtectMem0(C* obj, void (C::*func)())
			:m_Obj(obj),
			m_Func(func)
		{
		}
		~ProtectMem0()
		{
		}
		static VALUE FuncVoid(VALUE arg)
		{
			ProtectMem0 *self = (ProtectMem0*)arg;
			(self->m_Obj->*(self->m_Func))();
			return Qnil;
		}
		void operator()()
		{
			int state;
			rb_protect(ProtectMem0<void, C>::FuncVoid, (VALUE)this, &state);
			if(state != 0)
			{
				throw Error(state);
			}
		}
	private:
		C* m_Obj;
		T_func m_Func;
	};

	template<class R, class C, class A1> class ProtectMem1
	{
	public:
		typedef R (C::*T_func)(A1);
		ProtectMem1(C* obj, T_func func, A1 a1)
			:m_Obj(obj),
			m_Func(func),
			m_Arg1(a1)
		{
		}
		~ProtectMem1()
		{
		}
		static VALUE FuncR(VALUE arg)
		{
			ProtectMem1 *self = (ProtectMem1*)arg;
			R res = (self->m_Obj->*(self->m_Func))(self->m_Arg1);
			return (VALUE)res;
		}
		R operator()()
		{
			int state;
			VALUE res = rb_protect(ProtectMem1<R, C, A1>::FuncR, (VALUE)this, &state);
			if(state != 0)
			{
				throw Error(state);
			}
			return static_cast<R>(res);
		}
	private:
		C* m_Obj;
		T_func m_Func;
		A1 m_Arg1;
	};

	template<class C, class A1> class ProtectMem1<void, C, A1>
	{
	public:
		typedef void (C::*T_func)(A1);
		ProtectMem1(C* obj, T_func func, A1 a1)
			:m_Obj(obj),
			m_Func(func),
			m_Arg1(a1)
		{
		}
		~ProtectMem1()
		{
		}
		static VALUE FuncVoid(VALUE arg)
		{
			ProtectMem1 *self = (ProtectMem1*)arg;
			(self->m_Obj->*(self->m_Func))(self->m_Arg1);
			return Qnil;
		}
		void operator()()
		{
			int state;
			rb_protect(ProtectMem1<void, C, A1>::FuncVoid, (VALUE)this, &state);
			if(state != 0)
			{
				throw Error(state);
			}
		}
	private:
		C* m_Obj;
		T_func m_Func;
		A1 m_Arg1;
	};

	template<class R, class C> R ProtectMem(C* obj, R (C::*func)())
	{
		ProtectMem0<R, C> protect = ProtectMem0<R, C>(obj, func);
		return R(protect());
	}

	template<class C> void ProtectMem(C* obj, void (C::*func)())
	{
		ProtectMem0<void, C> protect = ProtectMem0<void, C>(obj, func);
		protect();
	}

	template<class R, class C, class A1> R ProtectMem(C* obj, R (C::*func)(A1), A1 a1)
	{
		ProtectMem1<R, C, A1> protect = ProtectMem1<R, C, A1>(obj, func, a1);
		return R(protect());
	}

	template<class C, class A1> void ProtectMem(C* obj, void (C::*func)(A1), A1 a1)
	{
		ProtectMem1<void, C, A1> protect = ProtectMem1<void, C, A1>(obj, func, a1);
		protect();
	}

// --- Protect static functions
// 2 arguments
	template<class R, class A1, class A2> class ProtectFun2
	{
	public:
		typedef R (*T_func)(A1, A2);
		ProtectFun2(T_func func, A1 a1, A2 a2)
			:m_Func(func),
			m_Arg1(a1),
			m_Arg2(a2)
		{
		}
		~ProtectFun2()
		{
		}
		static VALUE FuncR(VALUE arg)
		{
			ProtectFun2 *self = (ProtectFun2*)arg;
			R res = self->m_Func(self->m_Arg1, self->m_Arg2);
			return (VALUE) res;
		}
		R operator()()
		{
			int state;
			VALUE res = rb_protect(ProtectFun2<R, A1, A2>::FuncR, (VALUE)this, &state);
			if(state != 0)
			{
				throw Error(state);
			}
			return static_cast<R>(res);
		}
	private:
		T_func m_Func;
		A1 m_Arg1;
		A2 m_Arg2;
	};

	template<class A1, class A2> class ProtectFun2<void, A1, A2>
	{
	public:
		typedef void (*T_func)(A1, A2);
		ProtectFun2(T_func func, A1 a1, A2 a2)
			:m_Func(func),
			m_Arg1(a1),
			m_Arg2(a2)
		{
		}
		~ProtectFun2()
		{
		}
		static VALUE FuncV(VALUE arg)
		{
			ProtectFun2 *self = (ProtectFun2*)arg;
			self->m_Func(self->m_Arg1, self->m_Arg2);
			return Qnil;
		}
		void operator()()
		{
			int state;
			rb_protect(ProtectFun2<void, A1, A2>::FuncV, (VALUE)this, &state);
			if(state != 0)
			{
				throw Error(state);
			}
		}
	private:
		T_func m_Func;
		A1 m_Arg1;
		A2 m_Arg2;
	};

// 3 arguments
	template<class R, class A1, class A2, class A3> class ProtectFun3
	{
	public:
		typedef R (*T_func)(A1, A2, A3);
		ProtectFun3(T_func func, A1 a1, A2 a2, A3 a3)
			:m_Func(func),
			m_Arg1(a1),
			m_Arg2(a2),
			m_Arg3(a3)
		{
		}
		~ProtectFun3()
		{
		}
		static VALUE FuncR(VALUE arg)
		{
			ProtectFun3 *self = (ProtectFun3*)arg;
			R res = self->m_Func(self->m_Arg1, self->m_Arg2, self->m_Arg3);
			return (VALUE) res;
		}
		R operator()()
		{
			int state;
			VALUE res = rb_protect(ProtectFun3<R, A1, A2, A3>::FuncR, (VALUE)this, &state);
			if(state != 0)
			{
				throw Error(state);
			}
			return static_cast<R>(res);
		}
	private:
		T_func m_Func;
		A1 m_Arg1;
		A2 m_Arg2;
		A3 m_Arg3;
	};

	template<class A1, class A2, class A3> class ProtectFun3<void, A1, A2, A3>
	{
	public:
		typedef void (*T_func)(A1, A2, A3);
		ProtectFun3(T_func func, A1 a1, A2 a2, A3 a3)
			:m_Func(func),
			m_Arg1(a1),
			m_Arg2(a2),
			m_Arg3(a3)
		{
		}
		~ProtectFun3()
		{
		}
		static VALUE FuncV(VALUE arg)
		{
			ProtectFun3 *self = (ProtectFun3*)arg;
			self->m_Func(self->m_Arg1, self->m_Arg2, self->m_Arg3);
			return Qnil;
		}
		void operator()()
		{
			int state;
			rb_protect(ProtectFun3<void, A1, A2, A3>::FuncV, (VALUE)this, &state);
			if(state != 0)
			{
				throw Error(state);
			}
		}
	private:
		T_func m_Func;
		A1 m_Arg1;
		A2 m_Arg2;
		A3 m_Arg3;
	};

	template<class R, class A1, class A2> R ProtectFun(R (*func)(A1, A2), A1 a1, A2 a2)
	{
		ProtectFun2<R, A1, A2> protect = ProtectFun2<R, A1, A2>(func, a1, a2);
		return R(protect());
	}

	template<class A1, class A2> void ProtectFun(void (*func)(A1, A2), A1 a1, A2 a2)
	{
		ProtectFun2<void, A1, A2> protect = ProtectFun2<void, A1, A2>(func, a1, a2);
		protect();
	}

	template<class R, class A1, class A2, class A3> R ProtectFun(R (*func)(A1, A2, A3), A1 a1, A2 a2, A3 a3)
	{
		ProtectFun3<R, A1, A2, A3> protect = ProtectFun3<R, A1, A2, A3>(func, a1, a2, a3);
		return R(protect());
	}

	template<class A1, class A2, class A3> void ProtectFun(void (*func)(A1, A2, A3), A1 a1, A2 a2, A3 a3)
	{
		ProtectFun3<void, A1, A2, A3> protect = ProtectFun3<void, A1, A2, A3>(func, a1, a2, a3);
		protect();
	}
}

#endif

