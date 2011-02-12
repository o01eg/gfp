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

#ifndef _GPSTATIC_FUNCTIONS_H_
#define _GPSTATIC_FUNCTIONS_H_

#include <string>
#include <vm/environment.h>
#include <vm/object.h>

/// \brief Typed function data.
struct GPFunction
{
	std::string name; ///< Symbols name
	VM::FunctionPtr ptr; ///< Pointer to code
	unsigned char param; ///< Number of parameters
	unsigned char type_param; ///< Number of type parameters
};

#endif

