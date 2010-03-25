#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include <vector>
#include "object.h"
#include "environment.h"

namespace VM
{
	class Program
	{
	public:
		/// \brief Construct program from object.
		/// \param obj Object.
		Program(const Object& obj);

		/// \brief Construct program from file.
		/// \param env Environment.
		/// \param fn File name.
		Program(Environment &env, const std::string &fn);

		/// \brief Destroy program.
		~Program() {}

		/// \brief Saving program.
		/// \return Object.
		Object Save() const;

		/// \brief Get ADF.
		/// \param Number of ADF.
		/// \return ADF.
		Object GetADF(int num) const {return adfs[num];}
	private:
		std::vector<Object> adfs; ///< List of ADFs in program;
	};
}

#endif


