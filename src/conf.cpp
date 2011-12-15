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

#include "conf.h"

#include <fstream>
#include <locale>

#include "vm/ioobject.h"

Config::Config()
	:m_List(m_Env)
{
	std::ifstream f(DATA_DIR "config.lsp");
	f >> m_List;
	VM::WeakObject p(m_List);
	while(! p.IsNIL())
	{
		if(p.GetType() != VM::LIST)
		{
			break;
		}

		VM::WeakObject pair = p.GetHead();
		if(pair.GetType() == VM::LIST)
		{
			if(pair.GetHead().GetType() == VM::SYMBOL)
			{
				m_Options.insert(std::make_pair(m_Env.symbol_names[pair.GetHead().GetValue()], pair.GetTail().GetHead()));
			}
		}

		p = p.GetTail();
	}
}

Config::~Config()
{
}

signed long Config::GetSLong(const std::string& name, signed long def) const
{
	std::locale loc;
	std::string strup;
	for(std::string::const_iterator it = name.begin(); it != name.end(); ++ it)
	{
		strup += std::toupper(*it, loc);
	}

	std::map<std::string, VM::WeakObject>::const_iterator it = m_Options.find("CONFIG:" + strup);
	signed long res = def;
	if((it != m_Options.end()) && (it->second.GetType() == VM::INTEGER))
	{
		res = static_cast<signed long>(it->second.GetValue());
		std::cout << "Load option " << strup << " = " << res << std::endl;
	}
	else
	{
		std::cout << "Load default option " << strup << " = " << res << std::endl;
	}
	return res;
}


