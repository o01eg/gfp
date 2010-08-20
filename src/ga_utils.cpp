#include <stack>
#include <cstdlib>
#include "ga_utils.h"

const size_t MAX_DEPTH = 16;

bool GP::CheckForParam(const VM::Object &func)
{
	std::stack<VM::Object> stack;
	bool result = false;
	stack.push(func);
	while(! stack.empty())
	{
		const VM::Object& obj = stack.top();
		stack.pop();

		if(! obj.IsNIL())
		{
			if(obj.GetType() == VM::Object::LIST)
			{
				stack.push(obj.GetHead());
				stack.push(obj.GetTail());
			}
			if(obj.GetType() == VM::Object::PARAM)
			{
				return true;
			}
		}
	}
	return result;
}

VM::Object GP::GenerateObj(VM::Environment &env, const std::vector<std::pair<VM::Object, size_t> > &funcs, int depth)
{
	size_t choose = rand() % ((depth >= MAX_DEPTH) ? 5 : 6); // don't choose LIST on more depth.
	VM::Object res(env);
	switch(choose)
	{
	case 0: // integer
		res = VM::Object(env, VM::Object::INTEGER, static_cast<size_t>(rand() % 0x100001 - 0x80000));
		break;
	case 1: // callable objects
		res = funcs[rand() % funcs.size()].first;
		break;
	case 2: // NIL
		break;
	case 3: // parameter
		res = VM::Object(env, VM::Object::PARAM);
		break;
	case 4: // quote
		res = VM::Object(env, VM::Object::QUOTE);
		break;
	case 5: // list
		res = VM::Object(GP::GenerateObj(env, funcs, depth + 1), GP::GenerateObj(env, funcs, depth + 1));
		break;
	}
	return res;
}

VM::Object GP::GenerateExec(VM::Environment &env, const std::vector<std::pair<VM::Object, size_t> > &funcs, int depth)
{
	size_t choose = rand() % ((depth >= MAX_DEPTH) ? 3 : 5); // don't choose LIST on more depth.
	VM::Object res(env);
	switch(choose)
	{
	case 0: // integer
		res = VM::Object(env, VM::Object::INTEGER, static_cast<size_t>(rand() % 0x100001 - 0x80000));
		break;
	case 1: // nil
		break;
	case 2: // parameter
		res = VM::Object(env, VM::Object::PARAM);
		break;
	case 3: // call
		{
			size_t func_num = rand() % funcs.size();
			for(size_t param = 0; param < funcs[func_num].second; param ++)
			{
				res = VM::Object(GenerateExec(env, funcs, depth + 1), res);
			}
			res = VM::Object(funcs[func_num].first, res);
		}
		break;
	case 4: // ( ' object )
		res = VM::Object(GP::GenerateObj(env, funcs, depth + 1), res);
		res = VM::Object(VM::Object(env, VM::Object::QUOTE), res);
		break;

	}
	return res;
}


