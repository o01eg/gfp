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
