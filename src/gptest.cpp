#include "environment.h"
#include "ga_utils.h"
#include "ioobject.h"
#include "program.h"
#include "dot_dump.h"
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
			VM::Program prg1 = GP::GenerateProg(env, 6);
			VM::Program prg2 = GP::GenerateProg(env, 6);
			VM::Program mut = GP::MutateProg(prg1, 6);
			VM::Program cross = GP::CrossoverProg(prg1, prg2, 6);

			std::cout << "prog1 = " << prg1.Save() << std::endl;
			std::cout << "muta1 = " << mut.Save() << std::endl;
			std::cout << "prog2 = " << prg2.Save() << std::endl;
			std::cout << "cross = " << cross.Save() << std::endl;
			DotDump(env, "/tmp/1.dot");
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

