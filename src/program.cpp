/*
 * Copyright (C) 2010 O01eg <o01eg@yandex.ru> 
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

#include <fstream>
#include "program.h"
#include "ioobject.h"

using namespace VM;

Program::Program(const Object &obj)
	:m_Env(obj.GetEnv())
{
	Object head = obj.GetHead();
	Heap::UInt num_adfs = head.GetValue();
	m_ADFs.resize(num_adfs, Object(obj.GetEnv()));
	Object ptr = obj.GetTail();
	while(! ptr.IsNIL())
	{
		if((! ptr.GetHead().GetHead().IsNIL()) && (ptr.GetHead().GetHead().GetType() == ADF))
		{
			Heap::UInt num = ptr.GetHead().GetHead().GetValue();
			m_ADFs[num] = ptr.GetHead().GetTail().GetHead();
		}

		ptr = ptr.GetTail();
	}
}

Program::Program(Environment &env, const char *fn)
	:m_Env(env)
{
	std::ifstream f(fn);
	Object obj(env);
	f >> obj;
	this->~Program();
	new (this) Program(obj);
}

Object Program::Save() const
{
	if(m_ADFs.size() == 0)
	{
		THROW("No ADFs in program");
	}
	Environment &env = m_ADFs[0].GetEnv();
	Object res(env);
	signed long index;
	for(index = m_ADFs.size() - 1; index >= 0; index --)
	{
		if(! m_ADFs[index].IsNIL())
		{
			Object func_entry = Object(m_ADFs[index], Object(env));
			func_entry = Object(Object(env, ADF, index), func_entry);
			res = Object(func_entry, res);
		}
	}
	return Object(Object(env, INTEGER, m_ADFs.size()), res);
}

void Program::SetADF(int num, const Object& obj)
{
	if(m_ADFs.size() <= num)
	{
		m_ADFs.resize(num + 1, Object(m_Env));
	}
	m_ADFs[num] = obj;
}

void Program::Minimize()
{
	std::vector<bool> adf_calls(m_ADFs.size() * m_ADFs.size(), false);
	std::stack<VM::WeakObject> stack;
	size_t adf_index;
	for(adf_index = 0; adf_index < m_ADFs.size(); adf_index ++)
	{
		stack.push(m_ADFs[adf_index]);
		while(! stack.empty())
		{
			WeakObject temp = stack.top();
			stack.pop();
			if(! temp.IsNIL())
			{
				if((temp.GetType() == ADF) && (temp.GetValue() < m_ADFs.size()))
				{
					adf_calls[adf_index * m_ADFs.size() + temp.GetValue()] = true;
				}
				if(temp.GetType() == LIST)
				{
					stack.push(temp.GetHead());
					stack.push(temp.GetTail());
				}
			}
		}
	}

	for(adf_index = 1; adf_index < m_ADFs.size(); adf_index ++)
	{
		// check each function to calls from above ADF
		bool called = false;
		for(size_t adf_above_index = 0; adf_above_index < adf_index; adf_above_index ++)
		{
			if(adf_calls[adf_above_index * m_ADFs.size() + adf_index])
			{
				called = true;
				break;
			}
		}
		if(! called)
		{
			// remove ADF
			m_ADFs[adf_index] = Object(m_Env);
			for(size_t adf_above_index = 0; adf_above_index <= adf_index; adf_above_index ++)
			{
				adf_calls[adf_above_index * m_ADFs.size() + adf_index] = false;
			}
		}
	}
}

size_t Program::GetSettedADFs() const
{
	size_t res = 0;
	for(size_t adf_index = 0; adf_index < m_ADFs.size(); adf_index ++)
	{
		if(! m_ADFs[adf_index].IsNIL())
		{
			res ++;
		}
	}
	return res;
}

