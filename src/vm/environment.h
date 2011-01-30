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

#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "heap.h"
#include "stack.h"

namespace VM
{
	class Object;
	class WeakObject;
	class Program;
	class Symbols;

	typedef void (*FunctionPtr)(const Object& args, Object *result); ///< Pointer to LISP function.

	/// \brief Environment of execution.
	/// \todo Add hash table for functions name.
	/// \todo Fix memory leaks in heap.
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
		Stack stack; ///< Stack.
		std::vector<Func> functions; ///< Functions' list.

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

		/// \brief Get symbol object by name.
		/// \param[in] name Symbol's name.
		/// \return Object.
		const Object& GetSymbol(const std::string& name) const;

		/// \brief Get name of symbol.
		/// \param[in] obj Object.
		/// \return Name.
		const std::string& GetSymbol(const WeakObject& obj) const;
	private:
		/// \brief Execute Function.
		/// \param func_number Number of function in functions.
		/// \param ptr_obj_from_calc Pointer to stack of calculated expressions.
		/// \return Result of execution.
		Object CallFunction(Heap::UInt func_number, std::deque<Object> *ptr_obj_from_calc) const;

		/// \brief Generate list of arguments.
		/// \param param_number Nuber of parameters.
		/// \param ptr_obj_from_calc Pointer to stack of calculated expressions.
		/// \return List of arguments or NIL if error.
		Object GenerateArgsList(unsigned char param_number, std::deque<Object> *ptr_obj_from_calc) const;

#if _DEBUG_EVAL_
		/// \brief Dump content of stack.
		/// \param stack Stack of object.
		void DumpStack(const std::deque<Object> &stack) const;
#endif
		Program *m_Program; ///< Program executing in environment.

		static bool s_Stop; ///< Stop all evalations.

		Symbols *m_Symbols; ///< Symbols access;
	};
}

#endif

