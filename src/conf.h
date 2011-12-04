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

#include <map>

#include "vm/environment.h"
#include "vm/object.h"

/// \brief Signleton for access to config files.
class Config
{
public:
	static Config& Instance()
	{
		static Config conf;
		return conf;
	}

	signed long GetSLong(const std::string& name, signed long def) const;
private:
	Config();
	~Config();

	Config(const Config&); ///< Prevent copy-constructor
	Config& operator=(const Config&); ///< Prevent assing

	std::map<std::string, VM::WeakObject> m_Options; // Map of options
	VM::Environment m_Env; // Environment with options
	VM::Object m_List; // Pointer to options list
};

#endif

