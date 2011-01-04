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

#ifndef _SCHEME_SCHEME_H_
#define _SCHEME_SCHEME_H_

#include <libguile.h>
#include "catch.h"

class Scheme
{
public:
	static Scheme& Instance()
	{
		static Scheme scheme;
		return scheme;
	}

	SCM PrimitiveLoad(const char *filename);

	SCM LookUp(const char* symbol)
	{
		return scm_variable_ref(Catch(scm_c_lookup, symbol));
	}

	SCM Symbol(const char *name)
	{
		return scm_from_locale_symbol(name);
	}

	signed long HashRefSLong(SCM hash_table, const char *symbol, signed long def)
	{
		return Catch(StaticHashRefSLong, hash_table, Symbol(symbol), scm_from_long(def));
	}
private:
	Scheme();
	~Scheme();

	Scheme(const Scheme&);
	Scheme& operator=(const Scheme&);

	static signed long StaticHashRefSLong(SCM hash_table, SCM symbol, SCM def)
	{
		SCM value = scm_hashq_ref(hash_table, symbol, def);
		return scm_to_long(value);
	}
};

#endif

