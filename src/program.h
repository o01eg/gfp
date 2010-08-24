#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include <vector>
#include "object.h"
#include "environment.h"

namespace VM
{
	class Program
	{
	public:
		/// \brief Construct program from object.
		/// \param obj Object.
		Program(const Object& obj);

		/// \brief Construct empty program.
		/// \param env Environment.
		Program(Environment &env)
			:m_Env(env)
		{
		}

		/// \brief Construct program from file.
		/// \param env Environment.
		/// \param fn File name.
		Program(Environment &env, const char *fn);

		/// \brief Destroy program.
		~Program() {}

		/// \brief Saving program.
		/// \return Object.
		Object Save() const;

		/// \brief Get ADF.
		/// \param num Number of ADF.
		/// \return ADF.
		Object GetADF(int num) const
		{
			if(num < m_ADFs.size())
			{
				return m_ADFs[num];
			}
			return Object(m_Env);
		}

		/// \brief Set ADF.
		/// \param num Number of ADF.
		/// \param obj Source of ADF.
		void SetADF(int num, const Object& obj);

		/// \brief Get environment.
		/// \return Environment.
		Environment &GetEnv() const
		{
			return const_cast<Environment&>(m_Env);
		}
	private:
		std::vector<Object> m_ADFs; ///< List of ADFs in program.
		Environment &m_Env; ///< Environment.
	};
}

#endif


