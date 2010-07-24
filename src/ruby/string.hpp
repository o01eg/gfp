#ifndef _RUBY_STRING_H_
#define _RUBY_STRING_H_

#include <stdexcept>
#include "value.hpp"

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

