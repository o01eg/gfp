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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <iostream>
#include "scheme/scheme.h"

/// \brief Signleton for access to config files.
class Config
{
public:
	static Config& Instance()
	{
		static Config conf;
		return conf;
	}

	signed long GetSLong(const char *name, signed long def) const
	{
		signed long res = m_Scheme.HashRefSLong(m_HashTable, name, def);
		std::cout << "Load option " << name << " = " << res << std::endl;
		return res;
	}
private:
	Config();
	~Config();

	Config(const Config&); ///< 
	Config& operator=(const Config&);

	SCM m_HashTable;
	Scheme& m_Scheme;
};

#endif

