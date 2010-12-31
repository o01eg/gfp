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
#include "object.h"

#if _DEBUG_OBJECT_
#include "ioobject.h"
#endif

using namespace VM;

#if _DEBUG_OBJECT_
std::set<Object*> Object::m_AllObjects;
#endif

Object::Object(const Object &obj)
	:WeakObject(obj)
{
#if _DEBUG_OBJECT_
	m_AllObjects.insert(this);
#endif
	if(m_Pos)
	{
		GetEnv().heap.Attach(m_Pos);
	}
#if _DEBUG_OBJECT_
	//std::clog << "Create object " << *this;
#endif
}

#if 0
Object::Object(const WeakObject& obj)
	:WeakObject(obj)
{
#if _DEBUG_OBJECT_
	m_AllObjects.insert(this);
#endif
	if(m_Pos)
	{
		GetEnv().heap.Attach(m_Pos);
	}
}
#endif

Object::Object(const Environment &env, Types type)
	:WeakObject(env)
{
#if _DEBUG_OBJECT_
	m_AllObjects.insert(this);
#endif
#if _DEBUG_OBJECT_
	if((type != ERROR) && (type != PARAM) && (type != QUOTE) && (type != IF) && (type != EVAL))
	{
		THROW(Glib::ustring::compose("Object 0x%1: Non parameterless type %2.", this, type));
	}
#endif
	m_Pos = GetEnv().heap.Alloc(0x10 | (type & 0xf), 0);
//#if _DEBUG_OBJECT_
	if(! m_Pos)
	{
		THROW(Glib::ustring::compose("Object 0x%1: Couldn't alloc memory", this));
	}
	//std::clog << "Create object " << *this;
//#endif

}

Object::Object(const Environment &env, Types type, Heap::UInt value)
	:WeakObject(env)
{
#if _DEBUG_OBJECT_
	m_AllObjects.insert(this);
#endif
#if _DEBUG_OBJECT_
	if((type != INTEGER) && (type != FUNC) && (type != ADF))
	{
		THROW(Glib::ustring::compose("Object 0x%1: Non one-parameter type %2.", this, type));
	}
#endif
	m_Pos = GetEnv().heap.Alloc((value << 4) | 0x10 | (type & 0xf), value);
//#if _DEBUG_OBJECT_
	if(! m_Pos)
	{
		THROW(Glib::ustring::compose("Object 0x%1: Couldn't alloc memory", this));
	}
	//std::clog << "Create object " << *this;
//#endif

}

Object::Object(const Object& head, const Object& tail)
	:WeakObject(head.GetEnv())
{
#if _DEBUG_OBJECT_
	m_AllObjects.insert(this);
#endif
#if _DEBUG_OBJECT_
	if(&GetEnv() != &tail.GetEnv())
	{
		THROW(Glib::ustring::compose("Object 0x%1 and 0x%2: Different environment.", &head, &tail));
	}
#endif
	Heap::UInt hash = LIST;
	if(head.m_Pos)
	{
		hash = hash | (head.GetEnv().heap.At(head.m_Pos).hash << 4);
		GetEnv().heap.Attach(head.m_Pos);
	}
	if(tail.m_Pos)
	{
		hash = hash ^ (tail.GetEnv().heap.At(tail.m_Pos).hash << 4);
		GetEnv().heap.Attach(tail.m_Pos);
	}
	m_Pos = GetEnv().heap.Alloc(hash, head.m_Pos, tail.m_Pos);
//#if _DEBUG_OBJECT_
	if(! m_Pos)
	{
		THROW(Glib::ustring::compose("Object 0x%1: Couldn't alloc memory", this));
	}
	//std::clog << "Create object " << *this;
//#endif
}

Object::~Object()
{
#if _DEBUG_OBJECT_
	std::set<Object*>::iterator it = m_AllObjects.find(this);
	if(it != m_AllObjects.end())
	{
		m_AllObjects.erase(it);
	}
	else
	{
		std::cout << "Object 0x" << this << " It didn't be added into objects' list." << std::endl;
	}
#endif
	if(m_Pos)
	{
		if((GetType() == LIST) && (GetEnv().heap.At(m_Pos).count == 1))
		{
			/// \todo Use normal stack.
			std::stack<Heap::UInt> stack;
			stack.push(m_Pos);
			while(! stack.empty())
			{
				Heap::UInt elem_pos = stack.top();
				stack.pop();
				const Heap::Element &elem = GetEnv().heap.At(elem_pos);
				// add into stack only LIST objects with one link to them
				if((elem.hash & 0xf) == LIST)
				{
					if(elem.value)
					{
						if(GetEnv().heap.At(elem.value).count == 1)
						{
							stack.push(elem.value);
						}
						else
						{
							GetEnv().heap.Detach(elem.value);
						}
					}
					if(elem.tail)
					{
						if(GetEnv().heap.At(elem.tail).count == 1)
						{
							stack.push(elem.tail);
						}
						else
						{
							GetEnv().heap.Detach(elem.tail);
						}
					}
				}
				GetEnv().heap.Detach(elem_pos);
			}
		}
		else
		{
			GetEnv().heap.Detach(m_Pos);
		}
	}
}

Object& Object::operator=(const Object& obj)
{
	if(this != &obj)
	{
#if _DEBUG_OBJECT_
		if(&GetEnv() != &obj.GetEnv())
		{
			THROW(Glib::ustring::compose("Object 0x%1 and 0x%2: Different environments.", this, &obj));
		}
#endif
		if(m_Pos && GetType() == LIST) // If LIST before then use recursive destroing at destructor
		{
			this->~Object();
			new (this) Object(obj);
		}
		else
		{
			if(m_Pos)
			{
				GetEnv().heap.Detach(m_Pos);
			}
			m_Pos = obj.m_Pos;
			if(m_Pos)
			{
				GetEnv().heap.Attach(m_Pos);
			}
		}
	}
	return *this;
}

Object Object::GetObjectFrom(Environment &env, Heap::UInt pos)
{
	Object res(env); // create NIL object.
	if(pos)
	{
		res.m_Pos = pos;
		env.heap.Attach(pos);
	}
	return res;
}

Object Object::GetHead() const
{
#if _DEBUG_OBJECT_
	if(GetType() != LIST)
	{
		THROW(Glib::ustring::compose("Object 0x%1: Non LIST type %2.", this, GetType()));
	}
#endif
	return GetObjectFrom(GetEnv(), GetEnv().heap.At(m_Pos).value);
}

Object Object::GetTail() const
{
#if _DEBUG_OBJECT_
	if(GetType() != LIST)
	{
		THROW(Glib::ustring::compose("Object 0x%1: Non LIST type %2.", this, GetType()));
	}
#endif
	return GetObjectFrom(GetEnv(), GetEnv().heap.At(m_Pos).tail);
}

#if _DEBUG_OBJECT_
void Object::PrintObjects(Environment &env, std::ostream &os)
{
#if _DOT_MEMORY_
	for(std::set<Object*>::const_iterator it = m_AllObjects.begin(); it != m_AllObjects.end(); it ++)
	{
		if(&env == &((*it)->GetEnv()) && (*it)->m_Pos)
		{
			os << "\"" << (*it) << "\" -> " << (*it)->m_Pos << ";" << std::endl;
		}
	}
#else
	os << "All Objects:" << std::endl;
	for(std::set<Object*>::const_iterator it = m_AllObjects.begin(); it != m_AllObjects.end(); it ++)
	{
		if(&env == &((*it)->GetEnv()))
		{
			os << "Object: 0x" << (*it) << " pos:" << (*it)->m_Pos << std::endl;
		}
	}
#endif
}
#endif

