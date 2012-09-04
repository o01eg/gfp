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

#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include "object.h"

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
			: m_Env(env)
		{
		}

		/// \brief Copy-constructor for program.
		/// \param prog Program.
		Program(const Program& prog)
			: m_Env(prog.m_Env)
			, m_ADFs(prog.m_ADFs)
		{
		}

		/// \brief Move-constructor for program.
		/// \param prog Program.
		Program(Program&& prog)
			: m_Env(prog.m_Env)
			, m_ADFs(prog.m_ADFs)
		{
		}

		/// \brief Copy-assignment
		/// \param prog Program.
		/// \return Program.
		Program& operator=(const Program& prog)
		{
#if _DEBUG_OBJECT_
			if(&this->m_Env != &prog.m_Env)
			{
				THROW("Programs have different environments.");
			}
#endif
			if(this != &prog)
			{
				m_ADFs = prog.m_ADFs;
			}
			return *this;
		}

		/// \brief Copy-assignment
		/// \param prog Program.
		/// \return Program.
		Program& operator=(Program&& prog)
		{
#if _DEBUG_OBJECT_
			if(&this->m_Env != &prog.m_Env)
			{
				THROW("Programs have different environments.");
			}
#endif
			if(this != &prog)
			{
				m_ADFs = prog.m_ADFs;
			}
			return *this;
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
		/// \param num Index of ADF.
		/// \return ADF.
		Object GetADF(size_t num) const
		{
			if(num < m_ADFs.size())
			{
				return m_ADFs[num];
			}
			return m_Env.GetERROR();
		}

		/// \brief Get size.
		/// \return Number of ADFs.
		size_t GetSize() const
		{
			return m_ADFs.size();
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
			return m_Env;
		}
	private:
		Environment &m_Env; ///< Environment.
		std::vector<Object> m_ADFs; ///< List of ADFs in program.
	};
}

#endif


