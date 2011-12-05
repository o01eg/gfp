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
	bool not_width = width ? false : true;
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
				os << static_cast<signed long>(obj.GetValue()) << " ";
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
			case MACRO:
#if 1
				os << "@" << obj.GetValue() << " ";
#else
				os << obj.GetEnv().macroses[obj.GetValue()].name;
#endif
				break;
			case SYMBOL:
#if 0
				os << "&" << obj.GetValue() << " ";
#else
				os << obj.GetEnv().symbol_names[obj.GetValue()] << " ";
#endif
				break;
			case LIST:
				{
					signed long w = width;
					std::stack<bool> arrow_stack; // true - head, false - tail
					std::stack<WeakObject> stack;
					arrow_stack.push(true);
					stack.push(obj);
					WeakObject object(obj);
					bool arrow = true;
					//size_t level = 0;
					while((! stack.empty()) && (not_width || (w > 0)))
					{
						object = stack.top();
						stack.pop();
						arrow = arrow_stack.top();
						arrow_stack.pop();
						if(arrow) //head
						{
							if(object.IsNIL())
							{
								if(not_width || (w > 0))
								{
									os << "NIL ";
									w = std::max(0l, w - 4);
								}
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
									if(not_width || (w > 0))
									{
										os << "( ";
										w = std::max(0l, w - 2);
									}
									//level ++;
									stack.push(object.GetTail());
									arrow_stack.push(false);
									stack.push(object.GetHead());
									arrow_stack.push(true);
								}
								else
								{
									if(not_width || (w > 0))
									{
										os << std::setw(w) << object;
										w = std::max(0l, w - 2);
									}
								}
							}
						}
						else //tail
						{
							if(object.IsNIL())
							{
								if(not_width || (w > 0))
								{
									os << ") ";
									w = std::max(0l, w - 2);
								}
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
									if(not_width || (w > 0))
									{
										os << ". " << std::setw(w) << object << ") ";
										w = std::max(0l, w - 6);
									}
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
	if(width)
	{
		//ostr << "{" << width << "}";
		ostr.write(os.str().c_str(), std::min(static_cast<size_t>(width), os.str().size()));
	}
	else
	{
		ostr << os.str();
	}
	return ostr;
}

void dump_object(const WeakObject& obj, std::streamsize width)
{
	std::cout << std::setw(width) << obj << std::endl;
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
		reading = level > 0;
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

	VM::Object obj(env);
	if(env.GetObject(strup, &obj))
	{
		return obj;
	}
	else
	{
		switch(strup[0])
		{
			case '%':
				value = atol(strup.c_str() + 1);
				return Object(env, ADF, value);
			case '#':
				value = atol(strup.c_str() + 1);
				return Object(env, FUNC, value);
			case '@':
				value = atol(strup.c_str() + 1);
				return Object(env, MACRO, value);
			case '&':
				value = atol(strup.c_str() + 1);
				return Object(env, SYMBOL, value);
			default: // INTEGER
				if((strup[0] == '-') || ((strup[0] >= '0') && (strup[0] <= '9')))
				{
					value = atol(strup.c_str());
					return Object(env, INTEGER, value);
				}
				else // something strange, register new SYMBOL with name
				{
					return env.DefineSymbol(strup);
					//THROW("Unknown symbol " + strup);
				}
		}
	}
	return Object(env, ERROR);
}

