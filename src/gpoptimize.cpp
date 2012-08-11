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

#include <iostream>
#include <fstream>
#include "vm/environment.h"
#include "vm/program.h"
#include "vm/ioobject.h"
#include "libfunctions/functions.h"
#include "ga_utils.h"

int main(int argc, char** argv)
{
	if(argc == 2)
	{
		std::ifstream f(argv[1]);
		if(f.fail())
		{
			std::cerr << "Error." << std::endl;
			return -1;
		}
		VM::Environment env;
		VM::Environment::Func* array = func_array;
		while(array->func)
		{
			env.LoadFunction(array->name, array->number_param, array->func);
			++ array;
		}

		VM::Object obj(env);
		f >> obj;
		VM::Program prog(obj);
		for(size_t i = 0; i < prog.GetSize(); ++ i)
		{
			prog.SetADF(i, GP::Optimize(prog.GetADF(i), prog));
		}
		prog.Minimize();
		std::cout << "Opt: " << prog.Save() << std::endl;
	}
	else
	{
		std::cout << "Usage : " << argv[0] << " program.lsp" << std::endl;
	}
	return 0;
}

