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

#if _DEBUG_ENV_ || _DEBUG_EVAL_
#include <iostream>
#include <iomanip>
#include "ioobject.h"
#endif
#include <fstream>
#include <cstdlib>
#include <stack>
#include <deque>
#include <algorithm>
#include "environment.h"
#include "object.h"
#include "program.h"

using namespace VM;

const size_t MAX_DEPTH = 1536;

bool Environment::s_Stop = false;

Environment::Environment()
	:m_Program(0)
{
#if _DEBUG_ENV_
	std::clog << "Create environment " << this << std::endl;
#endif
}

Environment::~Environment()
{
#if _DEBUG_ENV_
	std::clog << "Destroy environment " << this << std::endl;
#endif
}

Object Environment::Eval(const Object &arg1, size_t *p_circle_counter) const
{
#if _DEBUG_ENV_
	std::clog << "Evalation object " << std::setw(40) << arg1 << " in " << this << std::endl;
#endif
	// create all needed stacks
	std::stack<Object> adf_params_obj;
	std::deque<Object> obj_to_calc, obj_from_calc;
	std::stack<Heap::UInt> adf_depth;
	bool is_in_adf = false;

	obj_to_calc.push_back(arg1);

	size_t circle_count = *p_circle_counter;

	while((! obj_to_calc.empty()) && circle_count && (! s_Stop))
	{
#if _DEBUG_EVAL_
		std::clog << "  --===--  " << std::endl;
#endif
		Heap::UInt depth = obj_to_calc.size();
		while(is_in_adf && (depth <= adf_depth.top()))
		{
#if _DEBUG_EVAL_
			std::clog << "Leave " << adf_depth.top() << std::endl;
#endif
			adf_params_obj.pop();
			adf_depth.pop();
			if(adf_depth.empty())
			{
				is_in_adf = false;
			}
		}

#if _DEBUG_EVAL_
		std::clog << "Depth: " << adf_depth.size() << std::endl;
		std::clog << "Objects to calculate:" << std::endl;
		DumpStack(obj_to_calc);
		std::clog << "Objects from calculate:" << std::endl;
		DumpStack(obj_from_calc);
#endif
		if(adf_depth.size() > MAX_DEPTH)
		{
#if _DEBUG_EVAL_
			std::clog << "Reach maximum depth." << std::endl;
#endif
			return Object(*this, ERROR);
		}
		Object obj = obj_to_calc.back();
		obj_to_calc.pop_back();

		if(obj.IsNIL() || (obj.GetType() == ERROR) || (obj.GetType() == INTEGER))
		{
			obj_from_calc.push_back(obj);
		}
		else
		{
			switch(obj.GetType())
			{
				case FUNC:
					{
						// make arguments list and call
						Object res = CallFunction(obj.GetValue(), &obj_from_calc);
						obj_from_calc.push_back(res);
					}
					break;
				case LIST:
					{
						Object head = obj.GetHead();
						if(head.IsNIL())
						{
							return Object(*this, ERROR);
						}
						else
						{
							switch(head.GetType())
							{
								case FUNC:
								case ADF:
								case LIST: // closure
									while((! obj.IsNIL()) && (obj.GetType() == LIST)) // while LIST isn't ended
									{
										head = obj.GetHead();
										obj_to_calc.push_back(head);
										obj = obj.GetTail();
									}
									break;
								case QUOTE:
									if((! obj.GetTail().IsNIL()) && (obj.GetTail().GetType() == LIST))
									{
										obj_from_calc.push_back(obj.GetTail().GetHead());
									}
									else
									{
										return Object(*this, ERROR);
									}
									break;
								case IF:
									if((! obj.GetTail().IsNIL()) && (obj.GetTail().GetType() == LIST) && (! obj.GetTail().GetTail().IsNIL()) && (obj.GetTail().GetTail().GetType() == LIST) && (! obj.GetTail().GetTail().GetTail().IsNIL()) && (obj.GetTail().GetTail().GetTail().GetType() == LIST))
									{
										Object cond = obj.GetTail().GetHead();
										Object otrue = obj.GetTail().GetTail().GetHead();
										Object ofalse = obj.GetTail().GetTail().GetTail().GetHead();
										obj_to_calc.push_back(ofalse);
										obj_to_calc.push_back(otrue);
										obj_to_calc.push_back(head);
										obj_to_calc.push_back(cond);
									}
									break;
								case EVAL:
									if((! obj.GetTail().IsNIL()) && (obj.GetTail().GetType() == LIST))
									{
										// move arg into to-calc stack.
										Object arg = obj.GetTail().GetHead();
										obj_to_calc.push_back(head);
										obj_to_calc.push_back(arg);
									}
									else
									{
										return Object(*this, ERROR);
									}
									break;
								default: // here get ERROR and INTEGER
									return Object(*this, ERROR);
									break;
							}
						}
					}
					break;
				case IF:
					if((! obj_from_calc.empty()) && (obj_to_calc.size() >= 2))
					{
						Object cond = obj_from_calc.back();
						obj_from_calc.pop_back();
						Object otrue = obj_to_calc.back();
						obj_to_calc.pop_back();
						Object ofalse = obj_to_calc.back();
						obj_to_calc.pop_back();
						if(cond.IsNIL())
						{
							// false
							obj_to_calc.push_back(ofalse);
						}
						else
						{
							if(cond.GetType() == ERROR)
							{
								return cond;
							}
							// true
							obj_to_calc.push_back(otrue);
						}
					}
					else
					{
						return Object(*this, ERROR);
					}
					break;
				case EVAL:
					if(! obj_from_calc.empty())
					{
						Object arg = obj_from_calc.back();
						obj_from_calc.pop_back();
						obj_to_calc.push_back(arg);
					}
					else
					{
						return Object(*this, ERROR);
					}
					break;
				case ADF:
					if(! is_in_adf)
					{
						is_in_adf = true;
					}
					if(obj_from_calc.empty())
					{
						// got ERROR
						return Object(*this, ERROR);
					}
					else
					{
						adf_params_obj.push(obj_from_calc.back());
						obj_from_calc.pop_back();
						adf_depth.push(obj_to_calc.size());
#if _DEBUG_EVAL_
						std::clog << "Enter " << adf_depth.top() << std::endl;
#endif
						if(m_Program->GetADF(obj.GetValue()).IsNIL())
						{
							return Object(*this, ERROR);
						}
						obj_to_calc.push_back(m_Program->GetADF(obj.GetValue()));
					}
					break;
				case PARAM:
					if(is_in_adf)
					{
						obj_from_calc.push_back(adf_params_obj.top());
					}
					else
					{
						return Object(*this, ERROR);
					}
					break;
				case QUOTE:
				default:
					return Object(*this, ERROR);
					break;
			}
		}

		circle_count --;
	}

	(*p_circle_counter) = circle_count;

	if(s_Stop)
	{
#if _DEBUG_ENV_
		std::clog << "Environment::Eval: Stop by interrupt." << std::endl;
#endif
		return Object(*this, ERROR);
	}

	if((obj_from_calc.size() == 1) && circle_count)
	{
#if _DEBUG_ENV_
		std::clog << "Result of evalatuon is " << std::setw(40) << obj_from_calc.back() << std::endl;
#endif
		return obj_from_calc.back();
	}
	else
	{
#if _DEBUG_ENV_
		std::clog << "Error in evalation" << std::endl;
#endif
		return Object(*this, ERROR);
	}
}

void Environment::LoadFunctionsFromArray(Func* array)
{
	while(array->func)
	{
		LoadFunction(array->name, array->number_param, array->func);
		array ++;
	}
}

FunctionPtr Environment::LoadFunction(const std::string &name, size_t argc, FunctionPtr ptr)
{
	std::vector<Func>::iterator it = std::find(functions.begin(), functions.end(), name);
	if(it == functions.end())
	{
		// not exist.
		if(ptr)
		{
			Environment::Func function;
			function.func = ptr;
			function.name = name;
			function.number_param = argc;
			functions.push_back(function);
		}
		return NULL;
	}
	else
	{
		FunctionPtr old_ptr = it->func;
		// exist
		if(ptr)
		{
			// replace
			it->func = ptr;
			it->number_param = argc;
		}
		else
		{
			functions.erase(it);
		}
		return old_ptr;
	}
	return NULL;
}

Object Environment::CallFunction(Heap::UInt func_number, std::deque<Object> *ptr_obj_from_calc) const
{
	const Func &function = functions[func_number];
#if _DEBUG_EVAL_
	std::clog << "Function " << function.name << " with " << static_cast<int>(function.number_param) << " args" << std::endl;
#endif
	Object args = GenerateArgsList(function.number_param, ptr_obj_from_calc);
#if _DEBUG_EVAL_
		std::clog << "args " << std::setw(40) << args << std::endl;
#endif
	if(args.IsNIL())
	{
		return Object(*this, ERROR);
	}
	Object result(*this);
	function.func(args, &result);
#if _DEBUG_EVAL_
	std::clog << " -> " << std::setw(40) << result << std::endl;
#endif
	return result;
}

Object Environment::GenerateArgsList(unsigned char param_number, std::deque<Object> *ptr_obj_from_calc) const
{
	std::deque<Object> &obj_from_calc = *ptr_obj_from_calc;
	Object args(*this);
	std::stack<Object> arguments;
	//check number of parameters and ERROR
	while(param_number)
	{
		if((! obj_from_calc.empty()) && (obj_from_calc.back().IsNIL() || (obj_from_calc.back().GetType() != ERROR)))
		{
#if _DEBUG_EVAL_
			std::clog << "arg " << std::setw(40) << obj_from_calc.back() << std::endl;
#endif
			arguments.push(obj_from_calc.back());
			obj_from_calc.pop_back();
			param_number --;
		}
		else
		{
#if _DEBUG_EVAL_
			if(obj_from_calc.empty())
			{
				std::clog << "arg end empty" << std::endl;
			}
			else
			{
				std::clog << "arg end " << std::setw(40) << obj_from_calc.back() << std::endl;
			}
#endif
			break;
		}
	}
	if(param_number)
	{
		return Object(*this);
	}
	while(! arguments.empty())
	{
		args = Object(arguments.top(), args);
		arguments.pop();
	}
	return args;
}

Object Environment::Run(const Object& param, size_t *p_circle_counter) const
{
	if(! m_Program)
	{
		THROW("Null pointer to program");
	}
	Object param_adf0(Object(*this, QUOTE), Object(param, Object(*this)));
	Object expr(Object(*this, ADF, 0), Object(param_adf0, Object(*this)));
	return Eval(expr, p_circle_counter);
}

#if _DEBUG_EVAL_
void Environment::DumpStack(const std::deque<Object> &stack) const
{
	std::clog << "Size: " << stack.size() << std::endl;
	for(std::deque<Object>::const_iterator it = stack.begin(); it != stack.end(); it ++)
	{
		std::clog << std::setw(40) << (*it) << std::endl;
	}
}
#endif

