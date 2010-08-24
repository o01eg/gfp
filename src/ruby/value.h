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

#ifndef _RUBY_VALUE_H_
#define _RUBY_VALUE_H_

#include <ruby.h>
extern "C" {
#include <ruby/encoding.h>
}

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

