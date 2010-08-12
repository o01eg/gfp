#include <iostream>
#include "environment.h"
#include "program.h"
#include "object.h"
#include "ioobject.h"

/// \brief Main Function.
/// \param argc Number of arguments.
/// \param argv Array of arguments.
/// \return Exit code.
int main(int argc, char **argv)
{
	if(argc != 2)
	{
		return -1;
	}
	try
	{
		VM::Environment env;
		env.LoadFunctions(DATA_DIR "functions.txt");
		try
		{
			VM::Program prog(env, argv[1]);
			env.program = &prog;
			const size_t MAX_CIRCLES = 1000;
			VM::Object a(env);
			std::cin >> a;
			env.circle_count = MAX_CIRCLES;
			std::cout << "result = " << env.Run(a) << std::endl;
			std::cout << "use " << (MAX_CIRCLES - env.circle_count) << " circles" << std::endl;
		}
		catch(Glib::Error &e)
		{
			std::cerr << "Catch Glib::Error in environment: " << e.what() << std::endl;
		}
	}
	catch(std::exception &e)
	{
		std::cerr << "Catch std::exception: " << e.what() << std::endl;
	}
	catch(Glib::Error &e)
	{
		std::cerr << "Catch Glib::Error: " << e.what() << std::endl;
	}

	return 0;
}

