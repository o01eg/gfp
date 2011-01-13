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

#ifndef _VM_SYMBOLS_H_
#define _VM_SYMBOLS_H_

#include <map>
#include <string>
#include "object.h"
#include "logger.h"

namespace VM
{
	class Symbols
	{
		typedef std::map<std::string, Object> Str2Obj;
		typedef std::map<WeakObject, std::string, WeakObject::HashCompareLess> Obj2Str;
	public:
		Symbols()
		{
		}
		~Symbols()
		{
		}
		void AppendSymbol(const std::string& str, const Object& obj)
		{
			std::pair<Str2Obj::iterator, bool> res_str2obj = m_Str2Obj.insert(std::make_pair(str, obj));
			res_str2obj.first->second = obj;
			std::pair<Obj2Str::iterator, bool> res_obj2str = m_Obj2Str.insert(std::make_pair(obj, str));
			res_obj2str.first->second = str;
		}
		const Object& GetObj(const std::string& str) const
		{
			Str2Obj::const_iterator it = m_Str2Obj.find(str);
			if(it == m_Str2Obj.end())
			{
				THROW(FormatString("Cann't find ", str, " symbol"));
			}
			return it->second;
		}
		const std::string& GetSym(const WeakObject& obj) const
		{
			Obj2Str::const_iterator it = m_Obj2Str.find(obj);
			if(it == m_Obj2Str.end())
			{
				THROW("Cann't find object");
			}
			return it->second;
		}
	private:
		Str2Obj m_Str2Obj;
		Obj2Str m_Obj2Str;
	};
}

#endif

