#ifndef _RUBY_ERROR_H_
#define _RUBY_ERROR_H_

#include <string>

namespace Ruby
{
	class Error
	{
	public:
		Error(int status);
		~Error()
		{
		}
		int GetStatus() const
		{
			return m_Status;
		}
		std::string GetMessage() const
		{
			return m_Msg;
		}
		std::string GetClass() const
		{
			return m_Class;
		}
	private:
		int m_Status;
		std::string m_Msg;
		std::string m_Class;
	};
}

#endif

