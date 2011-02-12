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

#include "types.h"

Types::Types(VM::Environment &env)
	:boolean(env.DefineSymbol("TYPE:BOOLEAN")),
	integer(env.DefineSymbol("TYPE:INTEGER")),
	list(env.DefineSymbol("TYPE:LIST")),
	pair(env.DefineSymbol("TYPE:PAIR")),
	any(env.DefineSymbol("TYPE:ANY")),
	func1(env.DefineSymbol("TYPE:FUNC1")),
	func2(env.DefineSymbol("TYPE:FUNC2")),
	func3(env.DefineSymbol("TYPE:FUNC3")),
	sym_div_by_zero(env.DefineSymbol("TYPE:SYM-DIV-BY-ZERO")),
	sym_empty_list(env.DefineSymbol("TYPE:SYM-EMPTY-LIST"))
{
}

