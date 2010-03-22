#if _DEBUG_ENV_
#include <iostream>
#include "ioobject.h"
#endif
#include <fstream>
#include <cstdlib>
#include <stack>
#include <glibmm/module.h>
#include "environment.h"
#include "object.h"

using namespace VM;

Environment::Environment()
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
	std::stack<Object> obj_to_calc, obj_from_calc, adf_params_obj;
	std::stack<Heap::UInt> adf_params;
	bool is_in_adf = false;

	obj_to_calc.push(arg1);

	while((! obj_to_calc.empty()) && (circle_count))
	{
		Object obj = obj_to_calc.top();
		obj_to_calc.pop();

		if(obj.IsNIL() || (obj.GetType() == Object::ERROR) || (obj.GetType() == Object::INTEGER))
		{
			obj_from_calc.push(obj);
		}
		else
		{
			switch(obj.GetType())
			{
				case Object::FUNC:
					// make arguments list and call
					obj_to_calc.push(CallFunction(obj.GetValue(), &obj_from_calc));
					break;
				case Object::LIST:
					{
						Object head = obj.GetHead();
						if(head.IsNIL())
						{
							obj_from_calc.push(Object(*this, Object::ERROR));
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
										obj_to_calc.push(head);
										obj = obj.GetTail();
									}
									break;
								case Object::QUOTE:
									obj_from_calc.push(obj.GetTail().GetHead());
									break;
								case Object::IF:
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
								default: // here get ERROR and INTEGER
									obj_from_calc.push(Object(*this, Object::ERROR));
									break;
							}
						}
					}
					break;
				case Object::ADF:
					/// \todo Write this.
					
				case Object::PARAM:
					/// \todo Write this.
				case Object::IF:
					/// \todo Write this.
				case Object::QUOTE:
				default:
					obj_from_calc.push(Object(*this, Object::ERROR));
					break;
			}
		}

		circle_count --;
	}
	if((! is_in_adf) && (obj_from_calc.size() == 1))
	{
#if _DEBUG_ENV_
		std::clog << "Result of evalatuon is " << obj_from_calc.top() << std::endl;
#endif
		return obj_from_calc.top();
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
}

Object Environment::CallFunction(Heap::UInt func_number, std::stack<Object> *ptr_obj_from_calc)
{
	const Func &function = functions[func_number];	
	Object args = GenerateArgsList(function.number_param, ptr_obj_from_calc);
	if(args.IsNIL())
	{
		return Object(*this, Object::ERROR);
	}
	Object result(*this);
	function.func(args, &result);
	return result;
}

Object Environment::GenerateArgsList(unsigned char param_number, std::stack<Object> *ptr_obj_from_calc)
{
	std::stack<Object> &obj_from_calc = *ptr_obj_from_calc;
	Object args(*this);
	//check number of parameters and ERROR
	while(param_number)
	{
		if((! obj_from_calc.empty()) && (obj_from_calc.top().IsNIL() || (obj_from_calc.top().GetType() != Object::ERROR)))
		{
			args = Object(obj_from_calc.top(), args);
			obj_from_calc.pop();
			param_number --;
		}
		else
		{
			break;
		}
	}
	if(param_number)
	{
		return Object(*this);
	}
	return args;
}

