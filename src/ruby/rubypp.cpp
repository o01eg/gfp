#include <stdexcept>
#include <iostream>
#include "rubypp.h"
#include "error.h"

Ruby::RubyImpl::RubyImpl()
{
	int ruby_argc = 1;
	char* ruby_argv[ruby_argc];
	ruby_argv[0] = "";
	char** r_argv = ruby_argv;
	std::clog << "Ruby starting" << std::endl;

	ruby_sysinit(&ruby_argc, &r_argv);
	RUBY_INIT_STACK;
	ruby_init();
	ruby_init_loadpath();
	ruby_script("mud");

	// Full Encoding support
	rb_enc_find_index("encdb");
	Require("enc/trans/transdb");

	std::clog << "Ruby started" << std::endl;
	
//	rb_enc_set_default_external(unknown value);
//	rb_enc_set_default_internal();
}

Ruby::RubyImpl::~RubyImpl()
{
	ruby_finalize();
	std::clog << "Ruby stoped" << std::endl;
}

void Ruby::RubyImpl::Require(const char *filename)
{
	std::clog << "Ruby: Require " << filename << std::endl;
	int state;
	VALUE fn = rb_str_new2(filename);
	OBJ_FREEZE(fn);
	rb_protect(Ruby::RubyImpl::_Require, fn, &state);
	if(state)
	{
		throw Error(state);
	}
}

VALUE Ruby::RubyImpl::_Require(VALUE fn)
{
	return rb_require_safe(fn, rb_safe_level());
}

Ruby::Value Ruby::RubyImpl::Execute(const char *code)
{
	int state;
	std::clog << "Ruby: " << code << std::endl;
	VALUE res = rb_eval_string_protect(code, &state);
	if(state)
	{
		throw Error(state);
	}
	std::clog << "Ruby: " << RSTRING_PTR(rb_obj_as_string(res)) << std::endl;
	return Value(res);
}

