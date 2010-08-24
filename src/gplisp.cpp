/*
 * Copyright (C) 2010 O01eg <o01eg@yandex.ru> 
 *
 *  This file is part of Genetic Function Programming.
 *
 *  Genetic Function Programming is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Genetic Function Programming is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Genetic Function Programming.  If not, see <http://www.gnu.org/licenses/>.
 */

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

