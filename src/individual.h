/*
 * Copyright (C) 2010-2014 O01eg <o01eg@yandex.ru>
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
#include <map>
#include "vm/program.h"
#include "vm/ioobject.h"
#include "ga_utils.h"

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
			ST_DIRS_MIN_3 = 0, ///< Number of used directions if less than 3 or 3.
			ST_AREA_SIZE, ///< Size of path's area.
			ST_DIRS, ///< Number of used directions.
			ST_STATIC_WORLD_ACCESS, ///< Static check for world access.
			ST_STATIC_MEMORY_ACCESS, ///< Static check for memory access.
			ST_GOOD_MOVES, ///< Count of good moves.
			ST_ALL_MOVES, ///< Count of any correct moves.
			ST_STEPS, ///< Number of steps.
			ST_MAX_ANSWER_QUALITY, ///< Max of move quality.
			ST_MIN_ANSWER_QUALITY, ///< Max of move quality.
			ST_COUNT_MEMORY, ///< Count of different memories.
			ST_ANSWER_QUALITY, ///< Aggregation of move quality.
			ST_NEG_ERROR_RESET, ///< Count of memory reset by ERROR.
			ST_NEG_RESET, ///< Count of memoty reset by not LIST.
			ST_STATIC_NEG_ERROR_TOTAL, ///< Sum of ERRORs.
			ST_STATIC_IF_TOTAL, ///< Sum of IFs.
			ST_NEG_CIRCLES, ///< Sum of least circles.
			ST_STATIC_FUNC_TOTAL, ///< Count of not-NIL ADFs.
			STATUS_VARIABLES ///< Mark of the end ResultStatus.
		};

		Result(int index)
			: m_Index(index)
		{
			std::fill(std::begin(m_Quality), std::end(m_Quality), 0);
		}
		Result(Result&& res)
			: m_Result(std::move(res.m_Result))
			, m_Index(res.m_Index)
		{
			std::copy(std::begin(res.m_Quality), std::end(res.m_Quality), std::begin(m_Quality));
		}
		Result(const Result& res)
		: m_Result(res.m_Result)
		, m_Index(res.m_Index)
		{
			std::copy(std::begin(res.m_Quality), std::end(res.m_Quality), std::begin(m_Quality));
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
					return m_Quality[i] > result.m_Quality[i];
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
				std::copy(std::begin(result.m_Quality), std::end(result.m_Quality), std::begin(m_Quality));
			}
			return *this;
		}
		Result& operator=(Result&& result)
		{
			if(this != &result)
			{
				m_Index = result.m_Index;
				m_Result = std::move(result.m_Result);
				std::copy(std::begin(result.m_Quality), std::end(result.m_Quality), std::begin(m_Quality));
			}
			return *this;
		}
		int GetIndex() const
		{
			return m_Index;
		}
		void Dump(std::ostream &os) const
		{
			os << "result = " << m_Result;
			os << "result[DIRS_MIN_3] = " << m_Quality[ST_DIRS_MIN_3] << std::endl;
			os << "result[AREA_SIZE] = " << m_Quality[ST_AREA_SIZE] << std::endl;
			os << "result[DIRS] = " << m_Quality[ST_DIRS] << std::endl;
			os << "result[STATIC_WORLD_ACCESS] = " << m_Quality[ST_STATIC_WORLD_ACCESS] << std::endl;
			os << "result[STATIC_MEMORY_ACCESS] = " << m_Quality[ST_STATIC_MEMORY_ACCESS] << std::endl;
			os << "result[GOOD_MOVES] = " << m_Quality[ST_GOOD_MOVES] << std::endl;
			os << "result[ALL_MOVES] = " << m_Quality[ST_ALL_MOVES] << std::endl;
			os << "result[STEPS] = " << m_Quality[ST_STEPS] << std::endl;
			os << "result[MAX_ANSWER_QUALITY] = " << m_Quality[ST_MAX_ANSWER_QUALITY] << std::endl;
			os << "result[MIN_ANSWER_QUALITY] = " << m_Quality[ST_MIN_ANSWER_QUALITY] << std::endl;
			os << "result[COUNT_MEMORY] = " << m_Quality[ST_COUNT_MEMORY] << std::endl;
			os << "result[ANSWER_QUALITY] = " << m_Quality[ST_ANSWER_QUALITY] << std::endl;
			os << "result[NEG_ERROR_RESET] = " << m_Quality[ST_NEG_ERROR_RESET] << std::endl;
			os << "result[NEG_RESET] = " << m_Quality[ST_NEG_RESET] << std::endl;
			os << "result[STATIC_NEG_ERROR_TOTAL] = " << m_Quality[ST_STATIC_NEG_ERROR_TOTAL] << std::endl;
			os << "result[STATIC_IF_TOTAL] = " << m_Quality[ST_STATIC_IF_TOTAL] << std::endl;
			os << "result[NEG_CIRCLES] = " << m_Quality[ST_NEG_CIRCLES] << std::endl;
			os << "result[STATIC_FUNC_TOTAL] = " << m_Quality[ST_STATIC_FUNC_TOTAL] << std::endl;
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
		std::copy(std::begin(m_Result.m_Quality), std::end(m_Result.m_Quality), std::begin(res.m_Quality));
		res.m_Result = m_Result.m_Result;
		return res;
	}

	void SetResult(const Result& res)
	{
		m_Result = res;
	}

	void SetResult(Result&& res)
	{
		m_Result = std::move(res);
	}

	void Optimize(const GP::OptimizeRules& rules);

	static std::vector<Result> Execute(VM::Environment &env, const std::vector<Individual>& population);
private:
	/// \brief Check move.
	/// \param[in] result Returned res for move.
	/// \param[out] directions Directions of move with power to them.
	/// \return Quality of move.
	static size_t CheckMove(const VM::WeakObject &result, std::map<int, signed long>& directions);

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

