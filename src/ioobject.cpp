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

#include <cstdlib>
#include <string>
#include <algorithm>
#include <locale>
#include <sstream>
#include <iomanip>
#include "ioobject.h"

using namespace VM;

/// \brief convert string to atomic object.
/// \param str String representation.
/// \param env Environment form creating object.
/// \return Atomic object.
Object str2atom(const std::string& str, Environment &env);

std::ostream& operator<<(std::ostream& ostr, const WeakObject& obj)
{
	std::streamsize width = ostr.width();
	ostr.width(0);
	std::stringstream os;
	/// \todo Use normal stack.
	if(! obj.IsNIL())
	{
		//const Environment& env = obj.GetEnv();
		switch(obj.GetType())
		{
			case ERROR:
				os << "ERROR ";
				break;
			case INTEGER:
				os << static_cast<int>(obj.GetValue()) << " ";
				break;
			case FUNC:
#if 0
				os << "#" << obj.GetValue() << " ";
#else
				os << obj.GetEnv().functions[obj.GetValue()].name << " ";
#endif
				break;
			case ADF:
				os << "%" << obj.GetValue() << " ";
				break;
			case PARAM:
				os << "$ ";
				break;
			case QUOTE:
				os << "QUOTE ";
				break;
			case IF:
				os << "IF ";
				break;
			case EVAL:
				os << "EVAL ";
				break;
			case LIST:
				{
					std::stack<bool> arrow_stack; // true - head, false - tail
					std::stack<WeakObject> stack;
					arrow_stack.push(true);
					stack.push(obj);
					WeakObject object(obj);
					bool arrow = true;
					//size_t level = 0;
					while(! stack.empty())
					{
						object = stack.top();
						stack.pop();
						arrow = arrow_stack.top();
						arrow_stack.pop();
						if(arrow) //head
						{
							if(object.IsNIL())
							{
								os << "NIL ";
							}
							else
							{
								if(object.GetType() == LIST)
								{
									//os << std::endl;
									/*for(size_t i = 0; i < level; i ++)
									{
										os << " ";
									}*/
									os << "( ";
									//level ++;
									stack.push(object.GetTail());
									arrow_stack.push(false);
									stack.push(object.GetHead());
									arrow_stack.push(true);
								}
								else
								{
									os << object;
								}
							}
						}
						else //tail
						{
							if(object.IsNIL())
							{
								os << ") ";
								//level --;
							}
							else
							{
								if(object.GetType() == LIST)
								{
									stack.push(object.GetTail());
									arrow_stack.push(false);
									stack.push(object.GetHead());
									arrow_stack.push(true);
								}
								else
								{
									os << ". " << object << ") ";
									//level --;
								}
							}
						}
					}
				}
				break;
			default:
				os << "ERROR ";
				break;
		}
	}
	else
	{
		os << "NIL ";
	}
	if(width < os.str().size())
	{
		//ostr << "{" << width << "}";
		ostr.write(os.str().c_str(), width);
	}
	else
	{
		ostr << os.str();
	}
	return ostr;
}

std::istream& operator>>(std::istream& is, Object& obj)
{
	/// \todo Use normal stack.
	/// \todo Add check for empty stacks.
	Environment& env = obj.GetEnv();
	std::stack<Object> obj_stack;
	std::stack<size_t> stack;
	std::stack<bool> point_stack;
	std::stack<bool> quote_stack;
	Object obj_temp(env);
	bool reading = true;
	bool point_pair = false;
	bool quote = false;
	std::string str;
	size_t level = 0;
	while(reading)
	{
		is >> str;
		switch(str[0])
		{
			case '(':
				level ++;
				point_stack.push(point_pair);
				point_pair = false;
				quote_stack.push(quote);
				quote = false;
				break;
			case ')':
				if(point_pair)
				{
					if((! stack.empty()) && (stack.top() == level))
					{
						obj_temp = obj_stack.top();
						obj_stack.pop();
						stack.pop();
					}
					else
					{
						THROW("Wrong S-expression.");
					}
				}
				else
				{
					obj_temp = Object(env); // NIL
				}
				while((! stack.empty()) && (stack.top() == level))
				{
					stack.pop();
					obj_temp = Object(obj_stack.top(), obj_temp);
					obj_stack.pop();
					if(stack.empty())
					{
						break;
					}
				}
				level --;
				point_pair = point_stack.top();
				point_stack.pop();
				quote = quote_stack.top();
				quote_stack.pop();
				if(quote)
				{
					obj_stack.push(Object(Object(env, QUOTE), Object(obj_temp, Object(env))));
					quote = false;
				}
				else
				{
					obj_stack.push(obj_temp);
				}
				stack.push(level);
				break;
			case '.':
				point_pair = true;
				break;
			case '\'':
				quote = true;
				break;
			default:
				obj_temp = str2atom(str, env);
				if(quote)
				{
					obj_stack.push(Object(Object(env, QUOTE), Object(obj_temp, Object(env))));
					quote = false;
				}
				else
				{
					obj_stack.push(obj_temp);
				}
				stack.push(level);
				break;

		}
		reading = static_cast<bool>(level);
	}
	obj = obj_stack.top();
	return is;
}

Object str2atom(const std::string& str, Environment &env)
{
	Heap::UInt value;
	std::locale loc;
	std::string strup;
	for(std::string::const_iterator it = str.begin(); it != str.end(); it ++)
	{
		strup += std::toupper(*it, loc);
	}
	std::vector<Environment::Func>::iterator ptr = std::find(env.functions.begin(), env.functions.end(), strup);
	if(ptr != env.functions.end())
	{
		return Object(env, FUNC, ptr - env.functions.begin());
	}
	else
	{
		switch(strup[0])
		{
			case 'E':
				if(strup == "EVAL")
				{
					return Object(env, EVAL);
				}
				return Object(env, ERROR);
			case '%':
				value = atol(strup.c_str() + 1);
				return Object(env, ADF, value);
			case '$':
				return Object(env, PARAM);
			case 'Q':
				return Object(env, QUOTE);
			case 'I':
				return Object(env, IF);
			case 'N':
				return Object(env);
			default: // INTEGER or ERROR
				if((strup[0] == '-') || ((strup[0] >= '0') && (strup[0] <= '9')))
				{
					value = atol(strup.c_str());
					return Object(env, INTEGER, value);
				}
				else
				{
					return Object(env, ERROR);
				}
		}
	}
	return Object(env, ERROR);
}

