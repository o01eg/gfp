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

