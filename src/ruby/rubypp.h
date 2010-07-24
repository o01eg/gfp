#ifndef _RUBY_RUBY_H_
#define _RUBY_RUBY_H_

#include "value.h"

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

