/*
 * Copyright (C) 2010-2011 O01eg <o01eg@yandex.ru> 
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
#include <iomanip>
#include <iostream>
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
			ST_DIR_CHANGES = 0, ///< Number of direction changes.
			ST_MOVE_CHANGES, ///< Number of move changes.
			ST_GOOD_MOVES, ///< Number of moves without collisions with walls.
			ST_SUM_MOVES, ///< Number of summary moves.
			ST_STATE_CHANGES, ///< Number when memory or world changes.
			ST_ANSWER_QUALITY, ///< Level of answer quality. (Aggregate by move)
			ST_ANSWER_CHANGES, ///< Numbers of different answers.
			ST_NEG_CIRCLES, ///< Sum of least circles.
			ST_MOVE_DIFF, ///< Different from 0 in move code. (Aggregate by move)
			ST_DIR_DIFF, ///< Different from 1-4 in direction. (Aggrerage by move)
			STATUS_VARIABLES
		};

		Result(int index)
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
		Result& operator=(const Result& result)
		{
			m_Index = result.m_Index;
			m_Result = result.m_Result;
			for(size_t i = 0; i < STATUS_VARIABLES; i ++)
			{
				m_Quality[i] = result.m_Quality[i];
			}
			return *this;
		}
		size_t GetIndex() const
		{
			return m_Index;
		}
		void Dump(std::ostream &os) const
		{
			os << std::setw(2) << m_Index << "[" << m_Quality[0];
			for(size_t j = 1; j < Individual::Result::STATUS_VARIABLES; j ++)
			{
				os << "," << m_Quality[j];
			}
			os << "]" << std::endl;
		}
		bool IsTested() const
		{
			return (m_Index != -1);
		}
		signed long m_Quality[STATUS_VARIABLES];
		std::string m_Result;
	private:
		int m_Index;
		
	};

	Individual(const Individual& ind)
		:m_ProgramText(ind.m_ProgramText),
		m_Result(ind.m_Result)
	{
	}

	/// \brief Desctructor.
	~Individual()
	{
	}

	Individual& operator=(const Individual& ind)
	{
		m_ProgramText = ind.m_ProgramText;
		m_Result = ind.m_Result;
		return *this;
	}

	VM::Program GetProgram(VM::Environment &env) const;
	const std::string& GetText() const
	{
		return m_ProgramText;
	}
	const Result& GetResult() const
	{
		return m_Result;
	}
	Result GetResult(int i) const
	{
		Result res(i);
		for(size_t j = 0; j < Result::STATUS_VARIABLES; j ++)
		{
			res.m_Quality[j] = m_Result.m_Quality[j];
		}
		res.m_Result = m_Result.m_Result;
		return res;
	}

	void SetResult(const Result& res)
	{
		m_Result = res;
	}

	static Individual GenerateRand(VM::Environment &env);

	static Individual Mutation(VM::Environment &env, const Individual& ind);

	static Individual Crossover(VM::Environment &env, const Individual& ind1, const Individual& ind2);
	static std::vector<Result> Execute(const std::vector<Individual> &population);

	/// \brief Load from file.
	/// \param[in] filename File name.
	/// \return Loaded individual.
	static Individual Load(const char* filename);

private:
	/// \brief Check move.
	/// \param[in] result Returned res for move.
	/// \param[out] code Code of move.
	/// \param[out] direction Direction of move.
	/// \return Quality of move.
	static size_t CheckMove(const VM::WeakObject &result, signed long *code, signed long *direction);

	/// \brief Create individual from program.
	/// \param prog LISP-program.
	Individual(const VM::Program &prog);

	Individual(); ///< Block for contsructor.

	std::string m_ProgramText; ///< Text of program.
	Result m_Result; ///< Result of last testing.
};

#endif

