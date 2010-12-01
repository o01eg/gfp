#ifndef _CURRENT_STATE_H_
#define _CURRENT_STATE_H_

#include <string>
#include <iostream>
#include <assert.h>

/// \brief Save current state of evalation
class CurrentState
{
public:
	/// \brief Dump current state
	static void Dump()
	{
		std::cerr << "Dump state:" << std::endl;
		std::cerr << " Generation: " << s_Generation << std::endl;
		std::cerr << " Program Text: " << s_Program << std::endl;
		std::cerr << std::endl;
	}

	static std::string s_Program; ///< Current evalating program.
	static size_t s_Generation; ///< Current generation.
};

#endif

