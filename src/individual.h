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

#ifndef _INDIVIDUAL_H_
#define _INDIVIDUAL_H_

#include <iomanip>
#include "vm/program.h"
#include "vm/ioobject.h"

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
			ST_DIRS = 0, ///< Number of used directions.
			ST_AREA_SIZE, ///< Size of path's area.
			ST_DIR_CHANGES, ///< Number of direction changes.
			ST_MOVE_CHANGES, ///< Number of move changes.
			ST_GOOD_MOVES, ///< Number of moves without collisions with walls.
			ST_SUM_MOVES, ///< Number of summary moves.
			ST_STATE_CHANGES, ///< Number when memory or world changes.
			ST_ANSWER_CHANGES, ///< Numbers of different answers.
			ST_ANSWER_QUALITY, ///< Level of answer quality. (Aggregate by move)
			ST_STATIC_WORLD_ACCESS, ///< Static check for world access.
			ST_STATIC_MEMORY_ACCESS, ///< Static check for memory access.
			ST_NEG_ERROR_TOTAL, ///< Sum of ERRORs.
			ST_NEG_CIRCLES, ///< Sum of least circles.
			ST_MOVE_DIFF, ///< Different from 0 in move code. (Aggregate by move)
			ST_DIR_DIFF, ///< Different from 1-4 in direction. (Aggrerage by move)
			ST_IF_TOTAL, ///< Count of IFs in program.
			STATUS_VARIABLES
		};

		Result(int index)
			:m_Index(index)
		{
			for(size_t i = 0; i < STATUS_VARIABLES; ++ i)
			{
				m_Quality[i] = 0;
			}
		}
		~Result()
		{
		}
		bool operator<(const Result& result) const
		{
			for(size_t i = 0; i < STATUS_VARIABLES; ++ i)
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
			if(this != &result)
			{
				m_Index = result.m_Index;
				m_Result = result.m_Result;
				for(size_t i = 0; i < STATUS_VARIABLES; ++ i)
				{
					m_Quality[i] = result.m_Quality[i];
				}
			}
			return *this;
		}
		Result& operator=(Result&& result)
		{
			if(this != &result)
			{
				m_Index = result.m_Index;
				m_Result = std::move(result.m_Result);
				for(size_t i = 0; i < STATUS_VARIABLES; ++ i)
				{
					m_Quality[i] = result.m_Quality[i];
				}
			}
			return *this;
		}
		size_t GetIndex() const
		{
			return m_Index;
		}
		void Dump(std::ostream &os) const
		{
			os << "result = " << m_Result;
			os << "result[DIRS] = " << m_Quality[ST_DIRS] << std::endl;
			os << "result[AREA_SIZE] = " << m_Quality[ST_AREA_SIZE] << std::endl;
			os << "result[DIR_CHANGES] = " << m_Quality[ST_DIR_CHANGES] << std::endl;
			os << "result[MOVE_CHANGES] = " << m_Quality[ST_MOVE_CHANGES] << std::endl;
			os << "result[GOOD_MOVES] = " << m_Quality[ST_GOOD_MOVES] << std::endl;
			os << "result[SUM_MOVES] = " << m_Quality[ST_SUM_MOVES] << std::endl;
			os << "result[STATE_CHANGES] = " << m_Quality[ST_STATE_CHANGES] << std::endl;
			os << "result[ANSWER_CHANGES] = " << m_Quality[ST_ANSWER_CHANGES] << std::endl;
			os << "result[ANSWER_QUALITY] = " << m_Quality[ST_ANSWER_QUALITY] << std::endl;
			os << "result[STATIC_WORLD_ACCESS] = " << m_Quality[ST_STATIC_WORLD_ACCESS] << std::endl;
			os << "result[STATIC_MEMORY_ACCESS] = " << m_Quality[ST_STATIC_MEMORY_ACCESS] << std::endl;
			os << "result[NEG_ERROR_TOTAL] = " << m_Quality[ST_NEG_ERROR_TOTAL] << std::endl;
			os << "result[NEG_CIRCLES] = " << m_Quality[ST_NEG_CIRCLES] << std::endl;
			os << "result[MOVE_DIFF] = " << m_Quality[ST_MOVE_DIFF] << std::endl;
			os << "result[DIR_DIFF] = " << m_Quality[ST_DIR_DIFF] << std::endl;
			os << "result[IF_TOTAL] = " << m_Quality[ST_IF_TOTAL] << std::endl;
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
		: m_Program(ind.m_Program)
		, m_Result(ind.m_Result)
		, m_Parents(ind.m_Parents)
	{
	}

	Individual(Individual&& ind)
		: m_Program(std::move(ind.m_Program))
		, m_Result(std::move(ind.m_Result))
		, m_Parents(std::move(ind.m_Parents))
	{
	}

	/// \brief Create individual from program.
	/// \param prog LISP-program.
	/// \param parents Parents of this individual.
	Individual(const VM::Program& prog, const std::vector<Individual>& parents)
	: m_Program(prog)
	, m_Result(-1) // new, not yet tested
	, m_Parents(parents)
	{
		Init();
	}

	/// \brief Create individual from program.
	/// \param prog LISP-program.
	/// \param parents Parents of this individual.
	Individual(const VM::Program& prog, std::vector<Individual>&& parents)
	: m_Program(prog)
	, m_Result(-1) // new, not yet tested
	, m_Parents(parents)
	{
		Init();
	}

	/// \brief Desctructor.
	~Individual()
	{
	}

	Individual& operator=(const Individual& ind)
	{
		if(this != &ind)
		{
			m_Program = ind.m_Program;
			m_Result = ind.m_Result;
			m_Parents = ind.m_Parents;
		}
		return *this;
	}

	Individual& operator=(Individual&& ind)
	{
		if(this != &ind)
		{
			m_Program = std::move(ind.m_Program);
			m_Result = std::move(ind.m_Result);
			m_Parents = std::move(ind.m_Parents);
		}
		return *this;
	}

	const VM::Program& GetProgram() const
	{
		return m_Program;
	}

	std::string GetText() const
	{
		std::stringstream ss;
		ss << m_Program.Save();
		return ss.str();
	}
	const Result& GetResult() const
	{
		return m_Result;
	}
	const std::vector<Individual>& GetParents() const
	{
		return m_Parents;
	}
	Result GetResult(int i) const
	{
		Result res(i);
		for(size_t j = 0; j < Result::STATUS_VARIABLES; ++ j)
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

	static std::vector<Result> Execute(VM::Environment &env, const std::vector<Individual>& population);

private:
	/// \brief Check move.
	/// \param[in] result Returned res for move.
	/// \param[out] code Code of move.
	/// \param[out] direction Direction of move.
	/// \return Quality of move.
	static size_t CheckMove(const VM::WeakObject &result, signed long *code, signed long *direction);

	void Init()
	{
		for(std::vector<Individual>::iterator parent = m_Parents.begin(); parent != m_Parents.end(); ++ parent)
		{
			parent->m_Parents.clear(); //don't store older parents.
		}
	}

	Individual() = delete; ///< Block for contsructor.

	VM::Program m_Program; ///< Text of program.
	Result m_Result; ///< Result of last testing.
	std::vector<Individual> m_Parents; ///< Parents of individual.
};

#endif

