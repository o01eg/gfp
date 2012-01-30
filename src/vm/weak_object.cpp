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

#include <stack>
#include "weak_object.h"

using namespace VM;

WeakObject& WeakObject::operator=(const WeakObject& obj)
{
	if(this != &obj)
	{
		if(&m_Env != &obj.m_Env)
		{
			THROW(FormatString("WeakObject 0x", this, " and 0x", &obj, ": Different environments."));
		}
		m_Pos = obj.m_Pos;
	}
	return *this;
}

Heap::UInt WeakObject::GetValue() const
{
#if _DEBUG_OBJECT_
	Types type = GetType();
	if((type != INTEGER) && (type != FUNC) && (type != ADF) && (type != SYMBOL) && (type != MACRO))
	{
		THROW(FormatString("Object 0x", this, ": Non one-parameter type ", type, "."));
	}
#endif
	return m_Env.heap.At(m_Pos).value;	
}

WeakObject WeakObject::GetHead() const
{
#if _DEBUG_OBJECT_
	if(GetType() != LIST)
	{
		THROW(FormatString("Object 0x", this, ": Non LIST type ", GetType(), "."));
	}
#endif
	return GetWeakObjectFrom(m_Env, m_Env.heap.At(m_Pos).value);
}

WeakObject WeakObject::GetTail() const
{
#if _DEBUG_OBJECT_
	if(GetType() != LIST)
	{
		THROW(FormatString("Object 0x", this, ": Non LIST type ", GetType(), "."));
	}
#endif
	return GetWeakObjectFrom(m_Env, m_Env.heap.At(m_Pos).tail);
}

WeakObject WeakObject::GetWeakObjectFrom(const Environment &env, Heap::UInt pos)
{
	WeakObject res(env);
	res.m_Pos = pos;
	return res;
}

bool WeakObject::operator==(const WeakObject& obj) const
{
#if _DEBUG_OBJECT_
	if(&m_Env != &obj.m_Env)
	{
		THROW(FormatString("WeakObject 0x", this, " and 0x", &obj, ": Different environments."));
	}
#endif
	if(m_Pos == obj.m_Pos)
	{
		return true;
	}
	else
	{
		if(IsNIL() || obj.IsNIL())
		{
			// one is NIL but another isn't
			return false;
		}
		else
		{
			if(m_Env.heap.At(m_Pos).hash == m_Env.heap.At(obj.m_Pos).hash)
			{
				/// \todo Rewrite this into low-level work with heap data.
				switch(GetType())
				{
					case ERROR:
					case PARAM:
					case QUOTE:
					case IF:
					case EVAL:
						return true;
					case INTEGER:
					case FUNC:
					case ADF:
					case MACRO:
					case SYMBOL:
						return GetValue() == obj.GetValue();
					case LIST:
						{
							std::stack<VM::WeakObject> stack;
							stack.push(*this);
							stack.push(obj);
							while(! stack.empty())
							{
								VM::WeakObject obj1 = stack.top();
								stack.pop();
								VM::WeakObject obj2 = stack.top();
								stack.pop();
								if(obj1.IsNIL())
								{
									if(! obj2.IsNIL())
									{
										return false;
									}
								}
								else
								{
									if(obj2.IsNIL())
									{
										return false;
									}
									else
									{
										if((obj1.GetType() == LIST) && (obj2.GetType() == LIST))
										{
											if(obj1.m_Pos != obj2.m_Pos)
											{
												stack.push(obj1.GetHead());
												stack.push(obj2.GetHead());
												stack.push(obj1.GetTail());
												stack.push(obj2.GetTail());
											}
										}
										else
										{
											if(obj1 != obj2)
											{
												return false;
											}
										}
									}
								}
							}
							return true;
						}
				}
			}
			else
			{
				// different hashes
				return false;
			}
		}
	}
	return false;
}


