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
			if(! t.IsNIL())
			{
				// if type is SYMBOL, then it defined type
				if(t.GetType() == VM::SYMBOL)
				{
					defined_types.push_back(t);
				}
				// if type is LIST then it possible unbinded type
				else if(t.GetType() == VM::LIST)
				{
					size_t unbinded_parameters = 0;

					// search on the all tree fo MACRO, check for single use it
					std::stack<VM::WeakObject> stack;
					stack.push(t);
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
									THROW(FormatString("Duplicate MACRO &", unbinded_parameters, " in ", t));
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
						THROW(FormatString("Wrong MACRO in ", t));
					}
					if(num_macros > 0)
					{
						unbinded_types.push_back(std::make_pair(t, num_macros));
					}
					else
					{
						defined_types.push_back(t);
					}
				}
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
