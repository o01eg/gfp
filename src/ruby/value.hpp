#ifndef _RUBY_VALUE_H_
#define _RUBY_VALUE_H_

#include <ruby.h>

namespace Ruby
{
	class Value
	{
	public:
		Value():m_Value(0) {}
		Value(const VALUE& val):m_Value(val) {}
		Value(const Value& val):m_Value(val.m_Value) {}

		operator VALUE() const
		{
			return m_Value;
		}

		~Value() {}

		Value GetInstanceVar(const char* name) const
		{
			return Value(rb_iv_get(m_Value, name));
		}
		int GetType() const
		{
			return rb_type(m_Value);
		}

		// for Hash type
		Value Hash_GetAref(Value key) const
		{
			return rb_hash_aref(m_Value, key.m_Value);
		}

		// for Fixnum type
		long Fixnum_GetLong() const
		{
			return FIX2LONG(m_Value);
		}
	protected:
		VALUE GetValue() const
		{
			return m_Value;
		}
	private:
		VALUE m_Value;
	};
}

#endif

