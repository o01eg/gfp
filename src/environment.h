#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "heap.h"
#include "stack.h"

namespace VM
{
	class Object;
	class Program;

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
		Program *program; ///< Program executing in environment.

		size_t circle_count; ///< Number of remain circles to prevent infinity loops.

		Environment(); ///< Constructor of environment.
		~Environment(); ///< Destructor of environment.
		
		/// \brief Evalation of Object.
		/// \param arg1 Argument.
		/// \return Result of evalation or ERROR.
		Object Eval(const Object &arg1);

		/// \brief Load Functions, described in filename.
		/// \todo Add upcasing of function's names.
		/// \param filename Path to file with functions.
		void LoadFunctions(const char* filename);

		/// \brief Run program with parameter.
		/// \param param Parameter.
		/// \return Result.
		Object Run(const Object& param);
	private:
		/// \brief Execute Function.
		/// \param func_number Number of function in functions.
		/// \param ptr_obj_from_calc Pointer to stack of calculated expressions.
		/// \return Result of execution.
		Object CallFunction(Heap::UInt func_number, std::deque<Object> *ptr_obj_from_calc);

		/// \brief Generate list of arguments.
		/// \param param_number Nuber of parameters.
		/// \param ptr_obj_from_calc Pointer to stack of calculated expressions.
		/// \return List of arguments or NIL if error.
		Object GenerateArgsList(unsigned char param_number, std::deque<Object> *ptr_obj_from_calc);

		/// \brief Dump content of stack.
		/// \param stack Stack of object.
		void DumpStack(const std::deque<Object> &stack);
	};
}

#endif
