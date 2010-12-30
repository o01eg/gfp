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

		/// \brief Remove unused ADFs.
		void Minimize();

		/// \brief Get ADF.
		/// \param num Number of ADF.
		/// \return ADF.
		Object GetADF(size_t num) const
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
		void SetADF(size_t num, const Object& obj);

		/// \brief Get number of not-NIL ADFs.
		/// \return Number of ADFs.
		size_t GetSettedADFs() const;

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


