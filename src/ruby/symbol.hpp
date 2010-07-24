#ifndef _RUBY_SYMBOL_HPP_
#define _RUBY_SYMBOL_HPP_

#include <stdexcept>
#include "value.hpp"
#include "string.hpp"

namespace Ruby
{
	class Symbol : public Value
	{
	public:
		Symbol(const String& str)
			:Value(rb_str_intern(str))
		{
		}
		Symbol(const char* str)
			:Value(ID2SYM(rb_intern(str)))
		{
		}
		~Symbol()
		{
		}
	private:
	};
}

#endif

