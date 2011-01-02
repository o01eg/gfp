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
	if((type != INTEGER) && (type != FUNC) && (type != ADF))
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
			if(GetType() == obj.GetType())
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
				// different types
				return false;
			}
		}
	}
	return false;
}


