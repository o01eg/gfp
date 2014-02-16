/*
 * Copyright (C) 2010-2014 O01eg <o01eg@yandex.ru>
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

#include <fstream>
#include <iomanip>
#include "program.h"
#include "ioobject.h"

using namespace VM;

Program::Program(const Object &obj)
	: m_CachedSave(obj)
{
	Object head = obj.GetHead();
	Heap::UInt num_adfs = head.GetValue();
	m_ADFs.resize(num_adfs, Object(obj.GetEnv()));
	Object ptr = obj.GetTail();
	while(! ptr.IsNIL())
	{
		if(! ptr.GetHead().GetHead().IsNIL())
		{
			if(ptr.GetHead().GetHead().GetType() == ADF)
			{
				Heap::UInt num = ptr.GetHead().GetHead().GetValue();
				if(num < m_ADFs.size())
				{
					m_ADFs[num] = ptr.GetHead().GetTail().GetHead();
				}
				else
				{
					std::stringstream ss;
					ss << obj;
					THROW("ADF's num out of range in " + ss.str());
				}
			}
			else if((ptr.GetHead().GetHead().GetType() == LIST) && (! ptr.GetHead().GetHead().GetHead().IsNIL()) && (ptr.GetHead().GetHead().GetHead().GetType() == ADF))
			{
				Heap::UInt num = ptr.GetHead().GetHead().GetHead().GetValue();
				if(num < m_ADFs.size())
				{
					m_ADFs[num] = ptr.GetHead().GetTail().GetHead();
				}
				else
				{
					std::stringstream ss;
					ss << obj;
					THROW("ADF's num out of range in " + ss.str());
				}
			}
		}

		ptr = ptr.GetTail();
	}
}

Program::Program(Environment &env, const char *fn)
	: m_CachedSave(env)
{
	std::ifstream f(fn);
	Object obj(env);
	f >> obj;
	this->~Program();
	new (this) Program(obj);
}

Object Program::InnerSave() const
{
	if(m_ADFs.size() == 0)
	{
		THROW("No ADFs in program");
	}
	Environment &env = m_CachedSave.GetEnv();
	Object res(env);
	signed long index;
	for(index = m_ADFs.size() - 1; index >= 0; index --)
	{
		if(! m_ADFs[index].IsNIL())
		{
			Object func_entry = Object(m_ADFs[index], Object(env));
			func_entry = Object(Object(env, ADF, index), std::move(func_entry));
			res = Object(std::move(func_entry), std::move(res));
		}
	}
	return Object(Object(env, INTEGER, m_ADFs.size()), std::move(res));
}

void Program::SetADF(size_t num, const Object& obj)
{
	m_CachedSave = Object(m_CachedSave.GetEnv()); // clean cache
	if(m_ADFs.size() <= num)
	{
		m_ADFs.resize(num + 1, Object(m_CachedSave.GetEnv()));
	}
	m_ADFs[num] = obj;
}

std::vector<unsigned int> Program::GetADFCallMap() const
{
	std::vector<unsigned int> adf_call_map(m_ADFs.size() * m_ADFs.size(), 0);
	std::stack<VM::WeakObject> stack;
	for(size_t adf_index = 0; adf_index < m_ADFs.size(); ++ adf_index)
	{
		stack.push(m_ADFs[adf_index]);
		while(! stack.empty())
		{
			WeakObject temp = stack.top();
			stack.pop();
			if(! temp.IsNIL())
			{
				if(temp.GetType() == ADF)
				{
					if(temp.GetValue() < m_ADFs.size())
					{
						++ adf_call_map[adf_index * m_ADFs.size() + temp.GetValue()];
					}
					else
					{
						std::stringstream ss;
						ss << Save();
						THROW(FormatString("Bugly generated program " + ss.str()));
					}
				}
				if(temp.GetType() == LIST)
				{
					stack.push(temp.GetHead());
					stack.push(temp.GetTail());
				}
			}
		}
	}

	return adf_call_map;
}

void Program::Minimize()
{
	std::vector<unsigned int> adf_call_map = GetADFCallMap();

#if 0
	// dump adf_calls
	for(adf_index = 0; adf_index < m_ADFs.size(); adf_index ++)
	{
		std::cout << std::setw(2) << adf_index << ":";
		for(size_t j = 0; j < m_ADFs.size(); j++)
		{
			if(adf_calls[adf_index * m_ADFs.size() + j])
			{
				std::cout << j << " ";
			}
		}
		std::cout << std::endl;
	}
#endif

	for(size_t adf_index = 1; adf_index < m_ADFs.size(); adf_index ++)
	{
		// check each function to calls from above ADF
		bool called = false;
		for(size_t adf_from_index = 0; adf_from_index < adf_index; ++ adf_from_index)
		{
			if(adf_call_map[adf_from_index * m_ADFs.size() + adf_index])
			{
				called = true;
				break;
			}
		}
		if(! called)
		{
			// remove ADF
			m_CachedSave = Object(m_CachedSave.GetEnv()); // clean cache

			m_ADFs[adf_index] = m_CachedSave;
			for(size_t adf_to_index = adf_index; adf_to_index < m_ADFs.size(); ++ adf_to_index)
			{
				adf_call_map[adf_index * m_ADFs.size() + adf_to_index] = 0;
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

