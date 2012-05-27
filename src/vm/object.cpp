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
#include "object.h"

#if _DEBUG_OBJECT_
#include "ioobject.h"
#endif

using namespace VM;

#if _DEBUG_OBJECT_
Object::Object(const Environment &env)
	:WeakObject(env)
{
	GetEnv().AllObjectsInstance().insert(this);
}
#else
Object::Object(const Environment &env)
	:WeakObject(env)
{
}
#endif

Object::Object(const Object &obj)
	:WeakObject(obj)
{
#if _DEBUG_OBJECT_
	GetEnv().AllObjectsInstance().insert(this);
#endif
	if(m_Pos)
	{
		GetEnv().heap.Attach(m_Pos);
	}
}

Object::Object(Object &&obj)
	:WeakObject(obj) // this.m_Pos = obj.m_Pos
{
#if _DEBUG_OBJECT_
	GetEnv().AllObjectsInstance().insert(this);
#endif
	obj.m_Pos = 0;
}

Object::Object(const Environment &env, Types type)
	:WeakObject(env)
{
#if _DEBUG_OBJECT_
	GetEnv().AllObjectsInstance().insert(this);

	if((type != ERROR) && (type != PARAM) && (type != QUOTE) && (type != IF) && (type != EVAL))
	{
		THROW(FormatString("Object 0x", this, ": Non parameterless type ", type, "."));
	}
#endif
	m_Pos = GetEnv().heap.Alloc(0x10 | (type & 0xf), 0);
	if(! m_Pos)
	{
		THROW(FormatString("Object 0x", this, ": Couldn't alloc memory"));
	}
}

Object::Object(const Environment &env, Types type, Heap::UInt value)
	:WeakObject(env)
{
#if _DEBUG_OBJECT_
	GetEnv().AllObjectsInstance().insert(this);
#endif
#if _DEBUG_OBJECT_
	if((type != INTEGER) && (type != FUNC) && (type != ADF) && (type != SYMBOL) && (type != MACRO))
	{
		THROW(FormatString("Object 0x", this, ": Non one-parameter type ", type, "."));
	}
#endif
	m_Pos = GetEnv().heap.Alloc((value << 4) | 0x10 | (type & 0xf), value);

	if(! m_Pos)
	{
		THROW(FormatString("Object 0x", this, ": Couldn't alloc memory"));
	}
}

Object::Object(const Object& head, const Object& tail)
	:WeakObject(head.GetEnv())
{
#if _DEBUG_OBJECT_
	GetEnv().AllObjectsInstance().insert(this);
#endif
#if _DEBUG_OBJECT_
	if(&GetEnv() != &tail.GetEnv())
	{
		THROW(FormatString("Object 0x", &head, " and 0x", &tail, ": Different environment."));
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
	if(! m_Pos)
	{
		THROW(FormatString("Object 0x", this, ": Couldn't alloc memory"));
	}
}

Object::~Object()
{
#if _DEBUG_OBJECT_
	std::set<Object*>::iterator it = GetEnv().AllObjectsInstance().find(this);
	if(it != GetEnv().AllObjectsInstance().end())
	{
		GetEnv().AllObjectsInstance().erase(it);
	}
	else
	{
		THROW(FormatString("Object 0x", this, " It didn't be added into objects' list."));
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
			THROW(FormatString("Object 0x", this, " and 0x", &obj, ": Different environments."));
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

Object& Object::operator=(Object&& obj)
{
	if(this != &obj)
	{
#if _DEBUG_OBJECT_
		if(&GetEnv() != &obj.GetEnv())
		{
			THROW(FormatString("Object 0x", this, " and 0x", &obj, ": Different environments."));
		}
#endif
		if(m_Pos && GetType() == LIST) // If LIST before then use recursive desroing at destructor
		{
			this->~Object();
			new (this) Object(obj); //move-constructor
		}
		else
		{
			if(m_Pos)
			{
				GetEnv().heap.Detach(m_Pos);
			}
			m_Pos = obj.m_Pos;
			obj.m_Pos = 0;
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
		THROW(FormatString("Object 0x", this, ": Non LIST type ", GetType(), "."));
	}
#endif
	return GetObjectFrom(GetEnv(), GetEnv().heap.At(m_Pos).value);
}

Object Object::GetTail() const
{
#if _DEBUG_OBJECT_
	if(GetType() != LIST)
	{
		THROW(FormatString("Object 0x", this, ": Non LIST type ", GetType(), "."));
	}
#endif
	return GetObjectFrom(GetEnv(), GetEnv().heap.At(m_Pos).tail);
}

#if _DEBUG_OBJECT_
void Object::PrintObjects(Environment &env, std::ostream &os)
{
#if _DOT_MEMORY_
	for(std::set<Object*>::const_iterator it = env.AllObjectsInstance().begin(); it != env.AllObjectsInstance().end(); ++ it)
	{
		os << "\"" << (*it) << "\" -> " << (*it)->m_Pos << ";" << std::endl;
	}
#else
	os << "All Objects:" << std::endl;
	for(std::set<Object*>::const_iterator it = env.AllObjectsInstance().begin(); it != env.AllObjectsInstance().end(); ++ it)
	{
		os << "Object: 0x" << (*it) << " pos:" << (*it)->m_Pos << std::endl;
	}
#endif
}
#endif

