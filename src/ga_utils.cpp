#include <stack>
#include <cstdlib>
#include "ga_utils.h"
#include "program.h"

const size_t MAX_DEPTH = 16;

bool GP::CheckForParam(const VM::Object &func)
{
	std::stack<VM::Object> stack;
	bool result = false;
	if(func.IsNIL())
	{
		return false;
	}
	if(func.GetType() != VM::Object::LIST)
	{
		return false;
	}
	stack.push(func);
	while(! stack.empty())
	{
		VM::Object obj = stack.top();
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

VM::Object GP::Mutation(const VM::Object& obj, bool is_exec, const std::vector<std::pair<VM::Object, size_t> > &funcs, int depth)
{
	VM::Object res(obj.GetEnv());
	if(is_exec)
	{
		if((rand() % 100) > 90)
		{
			res = GP::GenerateExec(obj.GetEnv(), funcs, depth);
		}
		else
		{
			// no change or go deeper
			if((! obj.IsNIL()) && (obj.GetType() == VM::Object::LIST))
			{
				// mutate arguments of function
				VM::Object head(obj.GetHead());
				if(head.IsNIL())
				{
					THROW("Head of callable list cann't be NIL");
				}
				if(head.GetType() == VM::Object::QUOTE)
				{
					// it's quote, make argument as non executable
					res = GP::Mutation(obj.GetTail().GetHead(), false, funcs, depth + 1);
					res = VM::Object(res, VM::Object(obj.GetEnv()));
				}
				else
				{
					std::stack<VM::Object> stack;
					VM::Object temp(obj.GetTail());
					while((! temp.IsNIL()) && (temp.GetType() == VM::Object::LIST))
					{
						stack.push(temp.GetHead());
						temp = temp.GetTail();
					}
					while(! stack.empty())
					{
						res = VM::Object(GP::Mutation(stack.top(), true, funcs, depth + 1), res);
						stack.pop();
					}
				}
				res = VM::Object(head, res);
			}
			else
			{
				res = obj;
			}
		}
	}
	else
	{
		// non exec mutation
		if((rand() % 100) > 90)
		{
			res = GP::GenerateObj(obj.GetEnv(), funcs, depth);
		}
		else
		{
			// no change or go deeper
			if((! obj.IsNIL()) && (obj.GetType() == VM::Object::LIST))
			{
				res = VM::Object(GP::Mutation(obj.GetHead(), false, funcs, depth + 1), GP::Mutation(obj.GetTail(), false, funcs, depth + 1));
			}
			else
			{
				res = obj;
			}
		}
	}
	return res;
}

VM::Program GP::GenerateProg(VM::Environment &env, size_t max_funcs)
{
	VM::Program res(env);
	std::vector<std::pair<VM::Object, size_t> > funcs;
	funcs.push_back(std::make_pair(VM::Object(env, VM::Object::IF), 3));
	for(size_t i = 0 ; i < env.functions.size(); i ++)
	{
		funcs.push_back(std::make_pair(VM::Object(env, VM::Object::FUNC, i), env.functions[i].number_param));
	}
	for(int adf_index = max_funcs; adf_index >= 0; adf_index --)
	{
		VM::Object adf(env);
		funcs.push_back(std::make_pair(VM::Object(env, VM::Object::ADF, adf_index), 1));
		do
		{
			adf = GP::GenerateExec(env, funcs, 0);
		}
		while(! GP::CheckForParam(adf));
		res.SetADF(adf_index, adf);
	}
	return res;
}

