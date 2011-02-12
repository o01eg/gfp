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

#ifndef _GPSTATIC_TYPES_H_
#define _GPSTATIC_TYPES_H_

#include <vm/environment.h>
#include <vm/object.h>

/// \brief Store type symbols
struct TypeSymbols
{
	VM::Object boolean;
	VM::Object integer;
	VM::Object list;
	VM::Object pair;
	VM::Object any;
	VM::Object func1;
	VM::Object func2;
	VM::Object func3;
	VM::Object sym_div_by_zero;
	VM::Object sym_empty_list;

	TypeSymbols(VM::Environment& env)
		:boolean(env),
		integer(env),
		list(env),
		pair(env),
		any(env),
		func1(env),
		func2(env),
		func3(env),
		sym_div_by_zero(env),
		sym_empty_list(env)
	{
	}
};

/// \brief Define all type symbols
/// \param[in,out] env Environment
void DefineTypes(VM::Environment &env);

#endif

