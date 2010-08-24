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
	:env(obj.env)
{
#if _DEBUG_OBJECT_
	m_AllObjects.insert(this);
#endif
	pos = obj.pos;
	if(pos)
	{
		env.heap.Attach(pos);
	}
#if _DEBUG_OBJECT_
	//std::clog << "Create object " << *this;
#endif
}

Object::Object(Environment &env_, Types type)
	:env(env_)
{
#if _DEBUG_OBJECT_
	m_AllObjects.insert(this);
#endif
#if _DEBUG_OBJECT_
	if((type != ERROR) && (type != PARAM) && (type != QUOTE) && (type != IF))
	{
		THROW(Glib::ustring::compose("Object 0x%1: Non parameterless type %2.", this, type));
	}
#endif
	pos = env.heap.Alloc(0x10 | (type & 0xf), 0);
#if _DEBUG_OBJECT_
	if(! pos)
	{
		THROW(Glib::ustring::compose("Object 0x%1: Couldn't alloc memory", this));
	}
	//std::clog << "Create object " << *this;
#endif

}

Object::Object(Environment &env_, Types type, Heap::UInt value)
	:env(env_)
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
	pos = env.heap.Alloc((value << 4) | 0x10 | (type & 0xf), value);
#if _DEBUG_OBJECT_
	if(! pos)
	{
		THROW(Glib::ustring::compose("Object 0x%1: Couldn't alloc memory", this));
	}
	//std::clog << "Create object " << *this;
#endif

}

Object::Object(const Object& head, const Object& tail)
	:env(head.env)
{
#if _DEBUG_OBJECT_
	m_AllObjects.insert(this);
#endif
#if _DEBUG_OBJECT_
	if(&env != &tail.env)
	{
		THROW(Glib::ustring::compose("Object 0x%1 and 0x%2: Different environment.", &head, &tail));
	}
#endif
	Heap::UInt hash = LIST;
	if(head.pos)
	{
		hash = hash | (head.env.heap.At(head.pos).hash << 4);
		env.heap.Attach(head.pos);
	}
	if(tail.pos)
	{
		hash = hash ^ (tail.env.heap.At(tail.pos).hash << 4);
		env.heap.Attach(tail.pos);
	}
	pos = env.heap.Alloc(hash, head.pos, tail.pos);
#if _DEBUG_OBJECT_
	if(! pos)
	{
		THROW(Glib::ustring::compose("Object 0x%1: Couldn't alloc memory", this));
	}
	//std::clog << "Create object " << *this;
#endif
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
	if(pos)
	{
		if((GetType() == LIST) && (env.heap.At(pos).count == 1))
		{
			/// \todo Use normal stack.
			std::stack<Heap::UInt> stack;
			stack.push(pos);
			while(! stack.empty())
			{
				Heap::UInt elem_pos = stack.top();
				stack.pop();
				const Heap::Element &elem = env.heap.At(elem_pos);
				// add into stack only LIST objects with one link to them
				if((elem.hash & 0xf) == LIST)
				{
					if(elem.value)
					{
						if(env.heap.At(elem.value).count == 1)
						{
							stack.push(elem.value);
						}
						else
						{
							env.heap.Detach(elem.value);
						}
					}
					if(elem.tail)
					{
						if(env.heap.At(elem.tail).count == 1)
						{
							stack.push(elem.tail);
						}
						else
						{
							env.heap.Detach(elem.tail);
						}
					}
				}
				env.heap.Detach(elem_pos);
			}
		}
		else
		{
			env.heap.Detach(pos);
		}
	}
}

Object& Object::operator=(const Object& obj)
{
	if(this != &obj)
	{
#if _DEBUG_OBJECT_
		if(&env != &obj.env)
		{
			THROW(Glib::ustring::compose("Object 0x%1 and 0x%2: Different environments.", this, &obj));
		}
#endif
		if(pos && GetType() == Object::LIST) // If LIST before then use recursive destroing at destructor
		{
			this->~Object();
			new (this) Object(obj);
		}
		else
		{
			if(pos)
			{
				env.heap.Detach(pos);
			}
			pos = obj.pos;
			if(pos)
			{
				env.heap.Attach(pos);
			}
		}
	}
	return *this;
}

Heap::UInt Object::GetValue() const
{
#if _DEBUG_OBJECT_
	Types type = GetType();
	if((type != INTEGER) && (type != FUNC) && (type != ADF))
	{
		THROW(Glib::ustring::compose("Object 0x%1: Non one-parameter type %2.", this, type));
	}
#endif
	return env.heap.At(pos).value;
}

Object Object::GetObjectFrom(Environment &env, Heap::UInt pos)
{
	Object res(env); // create NIL object.
	if(pos)
	{
		res.pos = pos;
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
	return GetObjectFrom(env, env.heap.At(pos).value);
}

Object Object::GetTail() const
{
#if _DEBUG_OBJECT_
	if(GetType() != LIST)
	{
		THROW(Glib::ustring::compose("Object 0x%1: Non LIST type %2.", this, GetType()));
	}
#endif
	return GetObjectFrom(env, env.heap.At(pos).tail);
}

bool Object::operator==(const Object& obj) const
{
#if _DEBUG_OBJECT_
	if(&env != &obj.env)
	{
		THROW(Glib::ustring::compose("Object 0x%1 and 0x%2: Different environments.", this, &obj));
	}
#endif
	if(pos == obj.pos)
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
						return true;
					case INTEGER:
					case FUNC:
					case ADF:
						return GetValue() == obj.GetValue();
					case LIST:
						return (GetHead() == obj.GetHead()) && (GetTail() == obj.GetTail());
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

#if _DEBUG_OBJECT_
void Object::PrintObjects(Environment &env, std::ostream &os)
{
#if _DOT_MEMORY_
	for(std::set<Object*>::const_iterator it = m_AllObjects.begin(); it != m_AllObjects.end(); it ++)
	{
		if(&env == &((*it)->env) && (*it)->pos)
		{
			os << "\"" << (*it) << "\" -> " << (*it)->pos << ";" << std::endl;
		}
	}
#else
	os << "All Objects:" << std::endl;
	for(std::set<Object*>::const_iterator it = m_AllObjects.begin(); it != m_AllObjects.end(); it ++)
	{
		if(&env == &((*it)->env))
		{
			os << "Object: 0x" << (*it) << " pos:" << (*it)->pos << std::endl;
		}
	}
#endif
}
#endif

