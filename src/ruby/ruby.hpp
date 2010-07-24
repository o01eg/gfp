#ifndef _RUBY_H_
#define _RUBY_H_

#include <ruby.h>
extern "C" {
#include <ruby/encoding.h>
}
#include "value.hpp"

namespace Ruby
{
	class RubyImpl
	{
	public:
		RubyImpl();
		~RubyImpl();

		void Require(const char *filename);
		Value Execute(const char *code);
	private:
		static VALUE _Require(VALUE fn);
		static VALUE _Execute(VALUE code);
	};
}

#endif

