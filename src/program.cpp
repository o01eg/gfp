#include <fstream>
#include "program.h"
#include "ioobject.h"

using namespace VM;

Program::Program(const Object &obj)
{
	Object head = obj.GetHead();
	Heap::UInt num_adfs = head.GetValue();
	adfs.resize(num_adfs, Object(obj.GetEnv()));
	Object ptr = obj.GetTail();
	while(! ptr.IsNIL())
	{
		if((! ptr.GetHead().GetHead().IsNIL()) && (ptr.GetHead().GetHead().GetType() == Object::ADF))
		{
			Heap::UInt num = ptr.GetHead().GetHead().GetValue();
			adfs[num] = ptr.GetHead().GetTail().GetHead();
		}

		ptr = ptr.GetTail();
	}
}

Program::Program(Environment &env, const char *fn)
{
	std::ifstream f(fn);
	Object obj(env);
	f >> obj;
	this->~Program();
	new (this) Program(obj);
}

Object Program::Save() const
{
	Environment &env = adfs[0].GetEnv();
	Object res(env);
	signed long index;
	for(index = adfs.size() - 1; index >= 0; index --)
	{
		if(! adfs[index].IsNIL())
		{
			Object func_entry = Object(adfs[index], Object(env));
			func_entry = Object(Object(env, Object::ADF, index), func_entry);
			res = Object(func_entry, res);
		}
	}
	return Object(Object(env, Object::INTEGER, adfs.size()), res);
}

