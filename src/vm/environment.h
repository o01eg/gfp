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

#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include <config.h>

#include <unordered_map>
#include <stack>
#include <memory>

#if _DEBUG_OBJECT_
#include <set>
#endif

#include "heap.h"

namespace VM
{
	class Object;
	class WeakObject;
	class Program;

	typedef void (*FunctionPtr)(const std::vector<Object>& args, Object *result); ///< Pointer to LISP function.

	/// \brief Environment of execution.
	class Environment
	{
	public:

		/// \brief Function data.
		struct Func
		{
			FunctionPtr func; ///< Pointer to callable function.
			std::string name; ///< Name of function.
			unsigned char number_param; ///< Number of parameters.

			/// \brief Constructor for static compilation
			Func(FunctionPtr func_, const std::string& name_, unsigned char number_param_)
				:func(func_),
				name(name_),
				number_param(number_param_)
			{
			}

			/// \brief Default constructor
			Func()
				:func(0)
			{
			}

			/// \brief Comparsion of names.
			/// \todo Change it into hash table.
			/// \param str String for comparsion.
			bool operator==(const std::string& str)
			{
				return (name == str);
			}
		};

		Heap heap; ///< Heap of objects.
		std::vector<Func> functions; ///< Functions' list.
		std::vector<std::string> symbol_names; ///< Symbols' list.

		Environment(); ///< Constructor of environment.
		~Environment(); ///< Destructor of environment.
		
		/// \brief Evalation of Object.
		/// \param[in] arg1 Argument.
		/// \param[in,out] p_circle_counter Circle counter.
		/// \return Result of evalation or ERROR.
		Object Eval(const Object &arg1, size_t *p_circle_counter) const;

		/// \brief Load Functions from array.
		/// \param[in] array Pointer into array of functions (must be default ended)
		void LoadFunctionsFromArray(Func* array);

		/// \brief Load or replace function.
		/// \param[in] name Function name
		/// \param[in] argc Number of agruments.
		/// \param[in] ptr Pointer tofunction.
		/// \return Old pointer or NULL.
		FunctionPtr LoadFunction(const std::string &name, size_t argc, FunctionPtr ptr);

		/// \brief Run program with parameter.
		/// \param[in] param Parameter.
		/// \param[in,out] p_circle_counter Circle counter.
		/// \return Result.
		Object Run(const Object& param, size_t *p_circle_counter) const;

		/// \brief Set current program.
		/// \param[in] prog Program.
		void SetProgram(VM::Program &prog)
		{
			m_Program = &prog;
		}

		/// \brief Stop all evalation.
		static void Stop()
		{
			s_Stop = true;
		}

		/// \brief Get object from symbols by name.
		/// \param[in] name Name of symbol.
		/// \param[out] obj Pointer to object.
		/// \return True if found name in map.
		bool GetObject(const std::string& name, Object* obj) const;

		/// \brief Set symbols with name and value
		/// \param[in] name Name of symbol.
		/// \param[in] obj Value of symbol.
		/// \return SYMBOL object.
		const Object& DefineSymbol(const std::string& name, const Object& obj);

		/// \brief Define symbol with NIL value.
		/// \param[in] name Name of symbol.
		/// \return SYMBOL object.
		const Object& DefineSymbol(const std::string& name);

#if _DEBUG_OBJECT_
		std::set<Object*>& AllObjectsInstance()
		{
			return m_AllObjects;
		}
#endif
	private:
		Environment(const Environment&) = delete; //Prevent copy-constructor.
		Environment(Environment&&) = delete; //Prevent move-constructor.
		Environment& operator=(const Environment&) = delete; //Prevent assign.

		/// \brief Execute Function.
		/// \param func_number Number of function in functions.
		/// \param ptr_obj_from_calc Pointer to stack of calculated expressions.
		/// \return Result of execution.
		Object CallFunction(Heap::UInt func_number, std::stack<Object> *ptr_obj_from_calc) const;

		/// \brief Generate list of arguments.
		/// \param param_number Nuber of parameters.
		/// \param ptr_obj_from_calc Pointer to stack of calculated expressions.
		/// \return Array of arguments which empty if error.
		std::vector<Object> GenerateArgsList(unsigned char param_number, std::stack<Object> *ptr_obj_from_calc) const;

#if _DEBUG_EVAL_
		/// \brief Dump content of stack.
		/// \param stack Stack of object.
		void DumpStack(const std::stack<Object> &stack) const;
#endif
		
#if _DEBUG_OBJECT_
		std::set<Object*> m_AllObjects; /// Set of all objects in this environment.
#endif

		Program *m_Program; ///< Program executing in environment.

		static bool s_Stop; ///< Stop all evalations.

		std::unique_ptr<std::unordered_map<std::string, Object> > m_Symbols; ///< String to object conversion.
		std::unique_ptr<std::vector<Object> > m_SymbolValues; ///< List of symbols values.
	};
}

#endif

