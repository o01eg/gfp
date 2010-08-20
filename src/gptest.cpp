#include "environment.h"
#include "ga_utils.h"
#include "ioobject.h"
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
		std::vector<std::pair<VM::Object, size_t> > funcs;
		funcs.push_back(std::make_pair(VM::Object(env, VM::Object::IF), 3));

		VM::Object res = GP::GenerateExec(env, funcs, 0);
		std::cout << "res: " << res << std::endl;
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

