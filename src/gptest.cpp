#include "environment.h"
#include "ga_utils.h"
#include "ioobject.h"
#include "program.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <glibmm/error.h>

int main(int argc, char **argv)
{
	srand(time(NULL));

	try
	{
		VM::Environment env;
		env.LoadFunctions(DATA_DIR "functions.txt");
		std::vector<std::pair<VM::Object, size_t> > funcs;
		funcs.push_back(std::make_pair(VM::Object(env, VM::Object::IF), 3));
		for(size_t i = 0 ; i < env.functions.size(); i ++)
		{
			funcs.push_back(std::make_pair(VM::Object(env, VM::Object::FUNC, i), env.functions[i].number_param));
		}

		VM::Object adf0 = GP::GenerateExec(env, funcs, 0);
		std::cout << "adf=" << adf0 << std::endl;
		VM::Object prog = VM::Object(adf0, VM::Object(env));
		prog = VM::Object(VM::Object(env, VM::Object::ADF, 0), prog);
		prog = VM::Object(prog, VM::Object(env));
		prog = VM::Object(VM::Object(env, VM::Object::INTEGER, 1), prog);
		VM::Program prg(prog);
		env.program = &prg;
		env.circle_count = 1000;
		VM::Object param = GP::GenerateObj(env, funcs, 0);
		VM::Object res = env.Run(param);
		std::cout << "res=" << res << std::endl;
	}
	catch(Glib::Error &e)
	{
		std::cout << "Glib::Error: " << e.what() << std::endl;
	}
	catch(std::exception &e)
	{
		std::cout << "std::exception: " << e.what() << std::endl;
	}

	return 0;
}

