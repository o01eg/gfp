/*
 * Copyright (C) 2010-2011 O01eg <o01eg@yandex.ru> 
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

#include "environment.h"
#include "ga_utils.h"
#include "ioobject.h"
#include "program.h"
#include "dot_dump.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "libfunctions/functions.h"

int main(int argc, char **argv)
{
	time_t seed = time(NULL);
	std::cout << "seed = " << seed << std::endl;
	srand(seed);

	try
	{
		VM::Environment env;
		{
			env.LoadFunctionsFromArray(func_array);
#if 1
			std::vector<std::pair<VM::Object, size_t> > funcs;
			funcs.push_back(std::make_pair(VM::Object(env, VM::IF), 3));
			funcs.push_back(std::make_pair(VM::Object(env, VM::EVAL), 1));
			for(size_t i = 0 ; i < env.functions.size(); i ++)
			{
				funcs.push_back(std::make_pair(VM::Object(env, VM::FUNC, i), env.functions[i].number_param));
			}
#endif
			std::cout << "Generate program... " << std::endl;
			VM::Program prg1 = GP::GenerateProg(env, 32);
			VM::Object arg = GP::GenerateObj(env, funcs, 32);
			env.SetProgram(prg1);
			size_t circle_count = 10000;
			VM::Object res = env.Run(arg, &circle_count);
#ifdef _DEBUG_HEAP_
			DotDump(env, "/tmp/1.dot");
#endif
		}
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

