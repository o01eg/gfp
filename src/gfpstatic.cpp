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
#include <vm/environment.h>
#include <vm/object.h>
#include <vm/ioobject.h>
#include <vm/logger.h>

std::pair<VM::Object, size_t> get_unbindes(const VM::Object& obj)
{
	std::pair<VM::Object, size_t> res = std::make_pair(obj, 0);
	if(! obj.IsNIL())
	{
		// if type is MACRO, then it self undinded
		if(obj.GetType() == VM::MACRO)
		{
			res.second = 1;
		}
		// if type is LIST then it possible unbinded type
		else if(obj.GetType() == VM::LIST)
		{
			size_t unbinded_parameters = 0;

			// search on the all tree for MACRO, check for single use it
			std::stack<VM::WeakObject> stack;
			stack.push(obj);
			size_t num_macros = 0;
			while(! stack.empty())
			{
				VM::WeakObject p = stack.top();
				stack.pop();

				if(! p.IsNIL())
				{
					if(p.GetType() == VM::MACRO)
					{
						if(p.GetValue() == unbinded_parameters)
						{
							THROW(FormatString("Duplicate MACRO &", unbinded_parameters, " in ", obj));
						}
						else if(p.GetValue() > unbinded_parameters)
						{
							unbinded_parameters = p.GetValue();	
						}
						++ num_macros;
					}
					else if(p.GetType() == VM::LIST)
					{
						stack.push(p.GetHead());
						stack.push(p.GetTail());
					}
				}
			}
			if(num_macros != unbinded_parameters)
			{
				THROW(FormatString("Wrong MACRO in ", obj));
			}
			res.second = num_macros;
		}
	}
	return res;
}

int main(int argc, char **argv)
{
	{
		VM::Environment env;

		//Load types from file
		std::ifstream f(DATA_DIR "static_types.lsp");
		if(f.fail())
		{
			THROW("Cann't open types list.");
		}
		VM::Object types_list(env);
		f >> types_list;
		std::cout << "Types: " << types_list << std::endl;

		std::vector<VM::Object> defined_types;
		std::vector<std::pair<VM::Object, size_t> > unbinded_types; // Type and number of unbinded parameters, for (TYPE:LIST @1) it is 1.

		VM::Object p(types_list);
		while((! p.IsNIL()) && (p.GetType() == VM::LIST))
		{
			VM::Object t = p.GetHead();
			std::pair<VM::Object, size_t> unbind = get_unbindes(t);
			if(unbind.second > 0)
			{
				unbinded_types.push_back(unbind);
			}
			else
			{
				defined_types.push_back(t);
			}

			p = p.GetTail();
		}



		size_t i;
		std::cout << "Defined types:" << std::endl;
		for(i = 0; i < defined_types.size(); ++ i)
		{
			std::cout << " " << defined_types[i] << std::endl;
		}

		std::cout << "Unbinded types:" << std::endl;
		for(i = 0; i < unbinded_types.size(); ++ i)
		{
			std::cout << " " << unbinded_types[i].first << " with " << unbinded_types[i].second << " params" << std::endl;
		}
	}
	return 0;
}
