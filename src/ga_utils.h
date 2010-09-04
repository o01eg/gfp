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

#ifndef _GA_UTILS_H_
#define _GA_UTILS_H_

#include "object.h"

/// \brief GP-related stuff.
namespace GP
{
	/// \brief Check for using params in function.
	/// \param func Function.
	/// \return Is have any params.
	bool CheckForParam(const VM::WeakObject &func);

	/// \brief Generate executable object.
	/// \param env Environment.
	/// \param funcs Array of callable objects.
	/// \param depth Depth.
	/// \return Generated object.
	VM::Object GenerateExec(VM::Environment &env, const std::vector<std::pair<VM::Object, size_t> > &funcs, int depth);

	/// \brief Generate any object.
	/// \param env Environment.
	/// \param funcs Array of callable objects.
	/// \param depth Depth.
	/// \return Generated object.
	VM::Object GenerateObj(VM::Environment &env, const std::vector<std::pair<VM::Object, size_t> > &funcs, int depth);

	/// \brief Mutation of objects.
	/// \param obj Object.
	/// \param is_exec Execatable object.
	/// \param funcs Array of callable objects.
	/// \param depth Depth.
	/// \return New object.
	VM::Object Mutation(const VM::Object& obj, bool is_exec, const std::vector<std::pair<VM::Object, size_t> > &funcs, int depth);

	/// \brief Generate new program.
	/// \param env Environment.
	/// \param max_funcs Maximum number of functions.
	/// \return Generated program.
	VM::Program GenerateProg(VM::Environment &env, size_t max_funcs);

	/// \brief Mutate program.
	/// \param prog Program.
	/// \param max_funcs Maximum number of function.
	/// \return Generated program.
	VM::Program MutateProg(const VM::Program &prog, size_t max_funcs);

	/// \brief Crossover programs.
	/// \param prog1 First program.
	/// \param prog2 Second program.
	/// \param max_funcs Maximum number of function.
	/// \return Generated program.
	VM::Program CrossoverProg(const VM::Program &prog1, const VM::Program &prog2, size_t max_funcs);
}

#endif

