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
	:m_Program(0),
	m_Symbols(new (std::nothrow) std::unordered_map<std::string, Object>),
	m_SymbolValues(new (std::nothrow) std::vector<Object>)
{
	if((m_Symbols == NULL) && (m_SymbolValues == NULL))
	{
		THROW("Cann't allocate environment.");
	}
	m_Symbols->insert(std::make_pair("NIL", Object(*this)));
	m_Symbols->insert(std::make_pair("$", Object(*this, PARAM)));
	m_Symbols->insert(std::make_pair("QUOTE", Object(*this, QUOTE)));
	m_Symbols->insert(std::make_pair("IF", Object(*this, IF)));
	m_Symbols->insert(std::make_pair("EVAL", Object(*this, EVAL)));

#if _DEBUG_ENV_
	//DefineSymbol("^_^", Object(*this, INTEGER, 12345));
	std::clog << "Create environment " << this << std::endl;
#endif
}

Environment::~Environment()
{
#if _DEBUG_ENV_
	std::clog << "Destroy environment " << this << std::endl;
#endif
	delete m_SymbolValues;
	delete m_Symbols;
}

Object Environment::Eval(const Object &arg1, size_t *p_circle_counter) const
{
#if _DEBUG_ENV_
	std::clog << "Evalation object " << std::setw(40) << arg1 << " in " << this << std::endl;
#endif
	// create all needed stacks
	std::stack<Object> adf_params_obj;
	std::stack<Object> obj_to_calc, obj_from_calc;
	std::stack<Heap::UInt> adf_depth;
	bool is_in_adf = false;

	obj_to_calc.push(arg1);

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
		Object obj = obj_to_calc.top();
		obj_to_calc.pop();

		if(obj.IsNIL() || (obj.GetType() == ERROR) || (obj.GetType() == INTEGER))
		{
			obj_from_calc.push(obj);
		}
		else
		{
			switch(obj.GetType())
			{
				case FUNC:
					{
						// make arguments list and call
						Object res = CallFunction(obj.GetValue(), &obj_from_calc);
						obj_from_calc.push(res);
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
										obj_to_calc.push(head);
										obj = obj.GetTail();
									}
									break;
								case QUOTE:
									if((! obj.GetTail().IsNIL()) && (obj.GetTail().GetType() == LIST))
									{
										obj_from_calc.push(obj.GetTail().GetHead());
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
										obj_to_calc.push(ofalse);
										obj_to_calc.push(otrue);
										obj_to_calc.push(head);
										obj_to_calc.push(cond);
									}
									break;
								case EVAL:
									if((! obj.GetTail().IsNIL()) && (obj.GetTail().GetType() == LIST))
									{
										// move arg into to-calc stack.
										Object arg = obj.GetTail().GetHead();
										obj_to_calc.push(head);
										obj_to_calc.push(arg);
									}
									else
									{
										return Object(*this, ERROR);
									}
									break;
								case SYMBOL:
									obj_to_calc.push(m_SymbolValues->at(head.GetValue()));
									obj = obj.GetTail();
									while((! obj.IsNIL()) && (obj.GetType() == LIST)) // while LIST isn't ended
									{
										head = obj.GetHead();
										obj_to_calc.push(head);
										obj = obj.GetTail();
									}
									break;
								case PARAM:
									if(is_in_adf)
									{
										obj_to_calc.push(adf_params_obj.top());
									}
									else
									{
										return Object(*this, ERROR);
									}
									obj = obj.GetTail();
									while((! obj.IsNIL()) && (obj.GetType() == LIST)) // while LIST isn't ended
									{
										head = obj.GetHead();
										obj_to_calc.push(head);
										obj = obj.GetTail();
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
						Object cond = obj_from_calc.top();
						obj_from_calc.pop();
						Object otrue = obj_to_calc.top();
						obj_to_calc.pop();
						Object ofalse = obj_to_calc.top();
						obj_to_calc.pop();
						if(cond.IsNIL())
						{
							// false
							obj_to_calc.push(ofalse);
						}
						else
						{
							if(cond.GetType() == ERROR)
							{
								return cond;
							}
							// true
							obj_to_calc.push(otrue);
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
						Object arg = obj_from_calc.top();
						obj_from_calc.pop();
						obj_to_calc.push(arg);
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
						adf_params_obj.push(obj_from_calc.top());
						obj_from_calc.pop();
						adf_depth.push(obj_to_calc.size());
#if _DEBUG_EVAL_
						std::clog << "Enter " << adf_depth.top() << std::endl;
#endif
						if(m_Program->GetADF(obj.GetValue()).IsNIL())
						{
							return Object(*this, ERROR);
						}
						obj_to_calc.push(m_Program->GetADF(obj.GetValue()));
					}
					break;
				case PARAM:
					if(is_in_adf)
					{
						obj_from_calc.push(adf_params_obj.top());
					}
					else
					{
						return Object(*this, ERROR);
					}
					break;
				case SYMBOL:
					obj_from_calc.push(m_SymbolValues->at(obj.GetValue()));
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
		std::clog << "Result of evalatuon is " << std::setw(40) << obj_from_calc.top() << std::endl;
#endif
		return obj_from_calc.top();
	}
#if _DEBUG_ENV_
	std::clog << "Error in evalation" << std::endl;
#endif
	return Object(*this, ERROR);
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
			m_Symbols->insert(std::make_pair(name, Object(*this, FUNC, functions.size() - 1)));
#if _DEBUG_ENV_ // use only for test aim
			//DefineSymbol("__" + name, Object(*this, FUNC, functions.size() - 1));
#endif
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

Object Environment::CallFunction(Heap::UInt func_number, std::stack<Object> *ptr_obj_from_calc) const
{
	const Func &function = functions[func_number];
#if _DEBUG_EVAL_
	std::clog << "Function " << function.name << " with " << static_cast<int>(function.number_param) << " args" << std::endl;
#endif
	std::vector<Object> args = GenerateArgsList(function.number_param, ptr_obj_from_calc);
	if(args.empty())
	{
#if _DEBUG_EVAL_
		std::clog << " -> ERROR" << std::endl;
#endif
		return Object(*this, ERROR);
	}
	Object result(*this);
	function.func(args, &result);
#if _DEBUG_EVAL_
	std::clog << " -> " << std::setw(40) << result << std::endl;
#endif
	return result;
}

std::vector<Object> Environment::GenerateArgsList(unsigned char param_number, std::stack<Object> *ptr_obj_from_calc) const
{
	std::stack<Object> &obj_from_calc = *ptr_obj_from_calc;
	std::vector<Object> arguments(param_number, Object(*this));

	//check number of parameters and ERROR while stay parameters
	while(param_number)
	{
		// if NIL or other type exclude ERROR
		if((! obj_from_calc.empty()) && (obj_from_calc.top().IsNIL() || (obj_from_calc.top().GetType() != ERROR)))
		{
#if _DEBUG_EVAL_
			std::clog << "arg " << std::setw(40) << obj_from_calc.top() << std::endl;
#endif
			arguments[param_number - 1] = obj_from_calc.top();
			obj_from_calc.pop();
			param_number --;
		}
		// if obj_from_calc is empty or get ERROR
		else
		{
#if _DEBUG_EVAL_
			if(obj_from_calc.empty())
			{
				std::clog << "arg end empty" << std::endl;
			}
			else
			{
				std::clog << "arg end " << std::setw(40) << obj_from_calc.top() << std::endl;
			}
#endif
			// then return NIL because cann't get right arguments
			arguments.clear();
			return arguments;
		}
	}

	return arguments;
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
void Environment::DumpStack(const std::stack<Object> &stack) const
{
	std::clog << "Size: " << stack.size() << std::endl;
	//for(std::stack<Object>::const_iterator it = stack.begin(); it != stack.end(); ++ it)
	if(! stack.empty())
	{
		std::clog << "Top: " << std::setw(40) << stack.top() << std::endl;
	}
}
#endif

bool Environment::GetObject(const std::string& name, Object* obj) const
{
	std::unordered_map<std::string, Object>::const_iterator it = m_Symbols->find(name);
	if(it == m_Symbols->end())
	{
		return false;
	}
	if(obj)
	{
		(*obj) = it->second;
	}
	return true;
}

const Object& Environment::DefineSymbol(const std::string& name, const Object& obj)
{
	//check name for wrong symbols: %(ADF), #(FUNC), @(MACRO), &(SYMBOL), '(QUOTE), (, ), "
	size_t found = name.find_first_of("%#@&\'()\"");
	if(found != std::string::npos)
	{
		THROW("Wrong symbol name " + name);
	}
	
	std::vector<std::string>::iterator it = std::find(symbol_names.begin(), symbol_names.end(), name);
	std::pair<std::unordered_map<std::string, Object>::iterator, bool> res;
	if(it == symbol_names.end())
	{
		Heap::UInt index = symbol_names.size();
		Object sym(*this, SYMBOL, index);
		res = m_Symbols->insert(std::make_pair(name, sym));
		m_SymbolValues->push_back(obj);
		symbol_names.push_back(name);
		if(! res.second)
		{
			res.first->second = sym;
		}
	}
	else
	{
		// if found this name
		Heap::UInt index = it - symbol_names.begin();
		Object sym(*this, SYMBOL, index);
		res = m_Symbols->insert(std::make_pair(name, sym));
		m_SymbolValues->at(index) = obj;
		res.first->second = sym;
	}
	return res.first->second;
}

const Object& Environment::DefineSymbol(const std::string& name)
{
	return DefineSymbol(name, Object(*this));
}

