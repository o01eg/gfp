#ifndef _GA_UTILS_H_
#define _GA_UTILS_H_

#include "object.h"

/// \brief GP-related stuff.
namespace GP
{
	/// \brief Check for using params in function.
	/// \param func Function.
	/// \return Is have any params.
	bool CheckForParam(const VM::Object &func);

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
}

#endif

