#include <stack>
#include "ga_utils.h"

using namespace GP;

bool CheckForParam(const VM::Object &func)
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

