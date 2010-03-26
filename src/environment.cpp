#if _DEBUG_ENV_
#include <iostream>
#include "ioobject.h"
#endif
#include <fstream>
#include <cstdlib>
#include <stack>
#include <deque>
#include <glibmm/module.h>
#include "environment.h"
#include "object.h"
#include "program.h"

using namespace VM;

Environment::Environment()
	:program(0)
{
#if _DEBUG_ENV_
	std::clog << "Create environment " << this << std::endl;
#endif
	circle_count = 0;
}

Environment::~Environment()
{
#if _DEBUG_ENV_
	std::clog << "Destroy environment " << this << std::endl;
#endif
}

Object Environment::Eval(const Object &arg1)
{
#if _DEBUG_ENV_
	std::clog << "Evalation object " << arg1 << " in " << this << std::endl;
#endif
	// create all needed stacks
	std::stack<Object> adf_params_obj;
	std::deque<Object> obj_to_calc, obj_from_calc;
	std::stack<Heap::UInt> adf_depth;
	bool is_in_adf = false;

	obj_to_calc.push_back(arg1);

	while((! obj_to_calc.empty()) && (circle_count))
	{
#if _DEBUG_EVAL_
		std::clog << "===" << std::endl;
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
		std::clog << "Objects to calculate:" << std::endl;
		DumpStack(obj_to_calc);
		std::clog << "Objects from calculate:" << std::endl;
		DumpStack(obj_from_calc);
#endif

		Object obj = obj_to_calc.back();
		obj_to_calc.pop_back();

		if(obj.IsNIL() || (obj.GetType() == Object::ERROR) || (obj.GetType() == Object::INTEGER))
		{
			obj_from_calc.push_back(obj);
		}
		else
		{
			switch(obj.GetType())
			{
				case Object::FUNC:
					// make arguments list and call
					obj_from_calc.push_back(CallFunction(obj.GetValue(), &obj_from_calc));
					break;
				case Object::LIST:
					{
						Object head = obj.GetHead();
						if(head.IsNIL())
						{
							obj_from_calc.push_back(Object(*this, Object::ERROR));
						}
						else
						{
							switch(head.GetType())
							{
								case Object::FUNC:
								case Object::ADF:
									while((! obj.IsNIL()) && (obj.GetType() == Object::LIST)) // while LIST isn't ended
									{
										head = obj.GetHead();
										obj_to_calc.push_back(head);
										obj = obj.GetTail();
									}
									break;
								case Object::QUOTE:
									obj_from_calc.push_back(obj.GetTail().GetHead());
									break;
								case Object::IF:
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
								default: // here get ERROR and INTEGER
									obj_from_calc.push_back(Object(*this, Object::ERROR));
									break;
							}
						}
					}
					break;
				case Object::IF:
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
							// true
							obj_to_calc.push_back(otrue);
						}
					}
					break;
				case Object::ADF:
					if(! is_in_adf)
					{
						is_in_adf = true;
					}
					adf_params_obj.push(obj_from_calc.back());
					obj_from_calc.pop_back();
					adf_depth.push(obj_to_calc.size());
#if _DEBUG_EVAL_
					std::clog << "Enter " << adf_depth.top() << std::endl;
#endif
					obj_to_calc.push_back(program->GetADF(obj.GetValue()));
					break;
				case Object::PARAM:
					if(is_in_adf)
					{
						obj_from_calc.push_back(adf_params_obj.top());
					}
					else
					{
						obj_from_calc.push_back(Object(*this, Object::ERROR));
					}
					break;
				case Object::QUOTE:
				default:
					obj_from_calc.push_back(Object(*this, Object::ERROR));
					break;
			}
		}

		circle_count --;
	}
	if(obj_from_calc.size() == 1)
	{
#if _DEBUG_ENV_
		std::clog << "Result of evalatuon is " << obj_from_calc.back() << std::endl;
#endif
		return obj_from_calc.back();
	}
	else
	{
#if _DEBUG_ENV_
		std::clog << "Error in evalation" << std::endl;
#endif
		return Object(*this, Object::ERROR);
	}
}

void Environment::LoadFunctions(const char* filename)
{
#if _DEBUG_ENV_
	std::clog << "Load functions from " << filename << std::endl;
#endif
	std::ifstream fin(filename);
	Glib::Module *module = static_cast<Glib::Module*>(0);
	try
	{
		while(!fin.eof())
		{
			std::string line;
			fin >> line;
			if(line[0]=='[')
			{
				//module
				std::string module_name;
				for(size_t i = 1; (i < line.size()) && (line[i] != ']'); i ++)
				{
					module_name += line[i];
				}
				if(module)
				{
					delete module;
					module = static_cast<Glib::Module*>(0);
				}
#if _DEBUG_ENV_
				std::clog << "Load module " << module_name << std::endl;
				std::clog << Glib::Module::build_path(LIB_DIR, module_name) << std::endl;
#endif
				module = new Glib::Module(Glib::Module::build_path(LIB_DIR, module_name), Glib::MODULE_BIND_LAZY);
#if _DEBUG_ENV_
				std::clog << module->get_name() << std::endl;
#endif
				module->make_resident();
			}
			else
			{
				if(module)
				{
					//function "func_00_eval:1:EVAL"
					Environment::Func function;
					std::string symbol;
					void* func_ptr;
					size_t i;
					for(i = 0; (i < line.size()) && (line[i] != ':'); i ++)
					{
						symbol += line[i];
					}
					if(module->get_symbol(symbol, func_ptr))
					{
						i++;
						function.func = reinterpret_cast<FunctionPtr>(func_ptr);
						function.number_param = atoi(line.c_str() + i);
						for(; (i < line.size()) && (line[i] != ':'); i ++);
						i++;
						for(; i < line.size(); i ++)
						{
							function.name += line[i];
						}
						if(function.name.size())
						{
#if _DEBUG_ENV_
							std::clog << "Load function " << function.name << " with " << static_cast<int>(function.number_param) << " parameters" << std::endl;
#endif
							functions.push_back(function);
						}
#if _DEBUG_ENV_
						else
						{
							std::clog << "No name" << std::endl;
						}
#endif
					}
#if _DEBUG_ENV_
					else
					{
						std::clog << "No symbol " << symbol << std::endl;
					}
#endif
				}
#if _DEBUG_ENV_
				else
				{
					std::clog << "No module" << std::endl;
				}
#endif
			}
		}
	}
	catch(...)
	{
		if(module)
		{
			delete module;
		}
		throw;
	}
	if(module)
	{
		delete module;
	}
}

Object Environment::CallFunction(Heap::UInt func_number, std::deque<Object> *ptr_obj_from_calc)
{
	const Func &function = functions[func_number];
#if _DEBUG_EVAL_
	std::clog << "Function " << function.name << " with " << static_cast<int>(function.number_param) << " args" << std::endl;
#endif
	Object args = GenerateArgsList(function.number_param, ptr_obj_from_calc);
#if _DEBUG_EVAL_
		std::clog << "args " << args << std::endl;
#endif
	if(args.IsNIL())
	{
		return Object(*this, Object::ERROR);
	}
	Object result(*this);
	function.func(args, &result);
#if _DEBUG_EVAL_
	std::clog << " -> " << result << std::endl;
#endif
	return result;
}

Object Environment::GenerateArgsList(unsigned char param_number, std::deque<Object> *ptr_obj_from_calc)
{
	std::deque<Object> &obj_from_calc = *ptr_obj_from_calc;
	Object args(*this);
	std::stack<Object> arguments;
	//check number of parameters and ERROR
	while(param_number)
	{
		if((! obj_from_calc.empty()) && (obj_from_calc.back().IsNIL() || (obj_from_calc.back().GetType() != Object::ERROR)))
		{
#if _DEBUG_EVAL_
			std::clog << "arg " << obj_from_calc.back() << std::endl;
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
				std::clog << "arg end " << obj_from_calc.back() << std::endl;
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

Object Environment::Run(const Object& param)
{
	if(! program)
	{
		THROW("Null pointer to program");
	}
	Object param_adf0(Object(*this, Object::QUOTE), Object(param, Object(*this)));
	Object expr(Object(*this, Object::ADF, 0), Object(param_adf0, Object(*this)));
	return Eval(expr);
}

void Environment::DumpStack(const std::deque<Object> &stack)
{
	for(std::deque<Object>::const_iterator it = stack.begin(); it != stack.end(); it ++)
	{
		std::clog << (*it) << std::endl;
	}
}
