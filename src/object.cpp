#include <stack>
#include "object.h"

#if _DEBUG_OBJECT_
#include "ioobject.h"
#endif

using namespace VM;

Object::Object(const Object &obj)
	:env(obj.env)
{
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
				/// \todo Write this.
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

