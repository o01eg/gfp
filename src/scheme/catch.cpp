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

#include <iostream>
#include "catch.h"

SCM CatchHandle(void *data, SCM key, SCM parameters)
{
	/// \todo Write this.
	SCM out = scm_current_output_port();
	std::cout << " Catch error!" << std::endl << "Key: ";
	std::cout.flush();
	scm_display(key, out);
	std::cout << std::endl << "Parameters: ";
	std::cout.flush();
	scm_display(parameters, out);
	std::cout << std::endl;
	return SCM_BOOL_T;
}

