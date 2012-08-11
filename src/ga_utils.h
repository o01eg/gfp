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

#ifndef _GA_UTILS_H_
#define _GA_UTILS_H_

#include "vm/object.h"

/// \brief GP-related stuff.
namespace GP
{
	/// \brief Count IFs in object exclude quoted.
	/// \param obj Object.
	/// \return Count of IFs.
	size_t CountIFs(const VM::WeakObject& obj);

	/// \brief Check if object contain evalable PARAM
	/// \param obj Object.
	/// \return Is PARAM contained.
	bool IsContainParam(const VM::WeakObject &obj);

	/// \brief Check for using params in function.
	/// \param func Function.
	/// \return Is have any params.
	bool CheckForParam(const VM::WeakObject &func);

	/// \brief Generate executable object.
	/// \param env Environment.
	/// \param funcs Array of callable objects.
	/// \param depth Depth.
	/// \param current_adf Current ADF.
	/// \return Generated object.
	VM::Object GenerateExec(VM::Environment &env, const std::vector<std::pair<VM::Object, size_t> > &funcs, size_t depth, size_t current_adf);

	/// \brief Reduce all constant parts.
	/// \param obj Object to reduction.
	/// \param prog Context of evalation.
	/// \return Optimized object.
	VM::Object Optimize(const VM::Object& obj, VM::Program& prog);

	/// \brief Generate any object.
	/// \param env Environment.
	/// \param funcs Array of callable objects.
	/// \param depth Depth.
	/// \param current_adf Current ADF.
	/// \return Generated object.
	VM::Object GenerateObj(VM::Environment &env, const std::vector<std::pair<VM::Object, size_t> > &funcs, size_t depth, size_t current_adf);

	/// \brief Mutation of objects.
	/// \param obj Object.
	/// \param is_exec Execatable object.
	/// \param funcs Array of callable objects.
	/// \param depth Depth.
	/// \param current_adf Current ADF.
	/// \return New object.
	VM::Object Mutation(const VM::Object& obj, bool is_exec, const std::vector<std::pair<VM::Object, size_t> > &funcs, size_t depth, size_t current_adf);

	/// \brief Generate new program.
	/// \param env Environment.
	/// \param funcs Functions for GA.
	/// \return Generated program.
	VM::Program GenerateProg(VM::Environment &env, const std::vector<std::pair<VM::Object, size_t> >& funcs);

	/// \brief Mutate program.
	/// \param prog Program.
	/// \param funcs Functions for GA.
	/// \return Generated program.
	VM::Program MutateProg(const VM::Program &prog, const std::vector<std::pair<VM::Object, size_t> >& funcs);

	/// \brief Crossover programs.
	/// \param prog1 First program.
	/// \param prog2 Second program.
	/// \return Generated program.
	VM::Program CrossoverProg(const VM::Program &prog1, const VM::Program &prog2);
}

#endif

