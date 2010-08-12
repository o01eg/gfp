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
}

#endif

