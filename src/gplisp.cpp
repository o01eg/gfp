/*
 * Copyright (C) 2010-2012 O01eg <o01eg@yandex.ru> 
 *
 * This file is part of Genetic Function Programming.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "vm/program.h"
#include "vm/ioobject.h"
#include "libfunctions/functions.h"

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
	{
		VM::Environment env;
		VM::Environment::Func* array = func_array;
		while(array->func)
		{
			env.LoadFunction(array->name, array->number_param, array->func);
			++ array;
		}

		{
			VM::Program prog(env, argv[1]);
			env.SetProgram(prog);
			const size_t MAX_CIRCLES = 100;
			size_t max_circles = MAX_CIRCLES;
			VM::Object a(env);
			std::cin >> a;
			std::cout << "result = " << env.Run(a, &max_circles) << std::endl;
			std::cout << "use " << (MAX_CIRCLES - max_circles) << " circles" << std::endl;
		}
	}

	return 0;
}

