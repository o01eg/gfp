#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include <vector>
#include "object.h"

namespace VM
{
	class Program
	{
	public:
		/// \brief Construct program from object.
		/// \param obj Object.
		Program(const Object& obj);

		/// \brief Destroy program.
		~Program() {}

		/// \brief Saving program.
		/// \return Object.
		Object Save() const;
	private:
		std::vector<Object> adfs; ///< List of ADFs in program;
	};
}

#endif


