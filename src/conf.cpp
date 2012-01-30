/*
 * Copyright (C) 2010-2012 O01eg <o01eg@yandex.ru> 
 *
 * This file is part of Genetic Function Programming.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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


