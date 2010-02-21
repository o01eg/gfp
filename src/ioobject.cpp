#include <cstdlib>
#include <string>
#include <algorithm>
#include "ioobject.h"

using namespace VM;

/// \brief convert string to atomic object.
/// \param str String representation.
/// \param env Environment form creating object.
/// \return Atomic object.
Object str2atom(const std::string& str, Environment &env);

std::ostream& operator<<(std::ostream& os, const Object& obj)
{
	/// \todo Use normal stack.
	/// \todo Use non-attached Object.
	if(! obj.IsNIL())
	{
		//const Environment& env = obj.GetEnv();
		switch(obj.GetType())
		{
			case Object::ERROR:
				os << "ERROR ";
				break;
			case Object::INTEGER:
				os << static_cast<int>(obj.GetValue()) << " ";
				break;
			case Object::FUNC:
#if 0
				os << "#" << obj.GetValue() << " ";
#else
				os << obj.GetEnv().functions[obj.GetValue()].name << " ";
#endif
				break;
			case Object::ADF:
				os << "%" << obj.GetValue() << " ";
				break;
			case Object::PARAM:
				os << "$ ";
				break;
			case Object::QUOTE:
				os << "\' ";
				break;
			case Object::IF:
				os << "? ";
				break;
			case Object::LIST:
				{
					std::stack<bool> arrow_stack; // true - head, false - tail
					std::stack<Object> stack;
					arrow_stack.push(true);
					stack.push(obj);
					Object object(obj);
					bool arrow = true;
					size_t level = 0;
					while(! stack.empty())
					{
						object = stack.top();
						stack.pop();
						arrow = arrow_stack.top();
						arrow_stack.pop();
						if(arrow) //head
						{
							if(object.IsNIL())
							{
								os << "NIL ";
							}
							else
							{
								if(object.GetType() == Object::LIST)
								{
									os << std::endl;
									for(size_t i = 0; i < level; i ++)
									{
										os << " ";
									}
									os << "( ";
									level ++;
									stack.push(object.GetTail());
									arrow_stack.push(false);
									stack.push(object.GetHead());
									arrow_stack.push(true);
								}
								else
								{
									os << object;
								}
							}
						}
						else //tail
						{
							if(object.IsNIL())
							{
								os << ") ";
								level --;
							}
							else
							{
								if(object.GetType() == Object::LIST)
								{
									stack.push(object.GetTail());
									arrow_stack.push(false);
									stack.push(object.GetHead());
									arrow_stack.push(true);
								}
								else
								{
									os << ". " << object << ") ";
									level --;
								}
							}
						}
					}
				}
				break;
			default:
				os << "unknown ";
				break;
		}
	}
	else
	{
		os << "NIL ";
	}
	return os;
}

std::istream& operator>>(std::istream& is, Object& obj)
{
	/// \todo Use normal stack.
	Environment& env = obj.GetEnv();
	std::stack<Object> obj_stack;
	std::stack<size_t> stack;
	std::stack<bool> point_stack;
	Object obj_temp(env);
	bool reading = true;
	bool point_pair = false;
	std::string str;
	size_t level = 0;
	while(reading)
	{
		is >> str;
		switch(str[0])
		{
			case '(':
				level ++;
				point_stack.push(point_pair);
				point_pair = false;
				break;
			case ')':
				if(point_pair)
				{
					if(stack.top() == level)
					{
						obj_temp = obj_stack.top();
						obj_stack.pop();
						stack.pop();
					}
					else
					{
						THROW("Wrong S-expression.");
					}
				}
				else
				{
					obj_temp = Object(env); // NIL
				}
				while(stack.top() == level)
				{
					stack.pop();
					obj_temp = Object(obj_stack.top(), obj_temp);
					obj_stack.pop();
					if(stack.empty())
					{
						break;
					}
				}
				level --;
				point_pair = point_stack.top();
				point_stack.pop();
				obj_stack.push(obj_temp);
				stack.push(level);
				break;
			case '.':
				point_pair = true;
				break;
			default:
				obj_temp = str2atom(str, env);
				obj_stack.push(obj_temp);
				stack.push(level);
				break;

		}
		reading = static_cast<bool>(level);
	}
	obj = obj_stack.top();
	return is;
}

Object str2atom(const std::string& str, Environment &env)
{
	Heap::UInt value;
	std::vector<Environment::Func>::iterator ptr = std::find(env.functions.begin(), env.functions.end(), str);
	if(ptr != env.functions.end())
	{
		return Object(env, Object::FUNC, ptr - env.functions.begin());
	}
	else
	{
		switch(str[0])
		{
			case 'E':
			case 'e':
				//return Object(env, Object::ERROR);
			case '#':
				//value = atol(str.c_str() + 1);
				return Object(env, Object::ERROR);
			case '%':
				value = atol(str.c_str() + 1);
				return Object(env, Object::ADF, value);
			case '$':
				return Object(env, Object::PARAM);
			case '\'':
				return Object(env, Object::QUOTE);
			case '?':
				return Object(env, Object::IF);
			case 'N':
			case 'n':
				return Object(env);
			default: // INTEGER
				value = atol(str.c_str());
				return Object(env, Object::INTEGER, value);
		}
	}
	return Object(env, Object::ERROR);
}

