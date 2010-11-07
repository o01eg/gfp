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

#ifndef _INDIVIDUAL_H_
#define _INDIVIDUAL_H_

#include <vector>
#include "program.h"

class Individual
{
public:
	/// \class Result
	/// \brief Result of execution of individual programs
	class Result
	{
	public:
		enum ResultStatus
		{
			ST_NEG_CIRCLES = 0,
			STATUS_VARIABLES
		};

		Result(size_t index)
			:m_Index(index)
		{
			for(size_t i = 0; i < STATUS_VARIABLES; i ++)
			{
				m_Quality[i] = 0;
			}
		}
		~Result()
		{
		}
		bool operator<(const Result& result) const
		{
			for(size_t i = 0; i < STATUS_VARIABLES; i ++)
			{
				if(m_Quality[i] == result.m_Quality[i])
				{
					continue;
				}
				else
				{
					if(m_Quality[i] > result.m_Quality[i])
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			return false;
		}
		size_t GetIndex() const
		{
			return m_Index;
		}
		int m_Quality[STATUS_VARIABLES];
		std::string m_Result;
	private:
		size_t m_Index;
		
	};

	Individual(const Individual& ind)
		:m_ProgramText(ind.m_ProgramText)
	{
	}

	/// \brief Desctructor.
	~Individual()
	{
	}

	Individual& operator=(const Individual& ind)
	{
		m_ProgramText = ind.m_ProgramText;
		return *this;
	}

	VM::Program GetProgram(VM::Environment &env) const;
	const std::string& GetText() const {return m_ProgramText;}

	static Individual GenerateRand(VM::Environment &env);

	static Individual Mutation(VM::Environment &env, const Individual& ind);

	static Individual Crossover(VM::Environment &env, const Individual& ind1, const Individual& ind2);
	static std::vector<Result> Execute(const std::vector<Individual> &population);

private:
	/// \brief Create individual from program.
	/// \param prog LISP-program.
	Individual(const VM::Program &prog);

	Individual(); ///< Block for contsructor.

	std::string m_ProgramText; ///< Text of program.
};

#endif

