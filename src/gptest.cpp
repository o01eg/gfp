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
	time_t seed = time(NULL);
	std::cout << "seed = " << seed << std::endl;
	srand(seed);

	try
	{
		VM::Environment env;
		{
			env.LoadFunctions(DATA_DIR "functions.txt");
			std::vector<std::pair<VM::Object, size_t> > funcs;
			funcs.push_back(std::make_pair(VM::Object(env, VM::Object::IF), 3));
			for(size_t i = 0 ; i < env.functions.size(); i ++)
			{
				funcs.push_back(std::make_pair(VM::Object(env, VM::Object::FUNC, i), env.functions[i].number_param));
			}

			std::cout << "Generate program... " << std::endl;
			VM::Program prg = GP::GenerateProg(env, 1);
			std::cout << prg.Save() << std::endl;
			VM::Object param = GP::GenerateObj(env, funcs, 0);
			std::cout << "param = " << param << std::endl;
			env.program = &prg;
			env.circle_count = 1000;
			VM::Object res = env.Run(param);
		}
	}
	catch(Glib::Error &e)
	{
		std::cout << "Glib::Error: " << e.what() << std::endl;
	}
	catch(std::exception &e)
	{
		std::cout << "std::exception: " << e.what() << std::endl;
	}
	catch(...)
	{
		std::cout << "unknown exception" << std::endl;
	}

	return 0;
}

