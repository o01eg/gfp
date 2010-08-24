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

#ifndef _RUBY_R_STRING_H_
#define _RUBY_R_STRING_H_

#include <stdexcept>
#include "value.h"

namespace Ruby
{
	class String : public Value
	{
	public:
		String(const char* str)
			:Value(rb_str_new2(str))
		{
		}

		String(const std::string& str)
			:Value(rb_str_new(str.c_str(), str.size()))
		{
		}


		String(const Value& val)
			:Value(val)
		{
			if(GetType() != T_STRING)
			{
				throw std::runtime_error("Cannot cast Value to String");
			}
		}

		String(const String& str)
			:Value(str)
		{
		}

		~String()
		{	
		}

		const char* GetCStr() const
		{
			return RSTRING_PTR(GetValue());
		}
		long GetLength() const
		{
			return RSTRING_LEN(GetValue());
		}
	private:
	};
}

#endif

