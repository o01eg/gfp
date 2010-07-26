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
		Result(size_t index)
			:m_Index(index)
		{
		}
		~Result()
		{
		}
		bool operator<(const Result& result) const
		{
			return true;
		}
		size_t GetIndex() const
		{
			return m_Index;
		}
	private:
		size_t m_Index;
	};

	Individual(const Individual& ind)
	{
		/// \todo Write this.
	}

	/// \brief Desctructor.
	~Individual()
	{
	}

	Individual& operator=(const Individual& ind)
	{
		/// \todo Write this.
		return *this;
	}

	static Individual GenerateRand(VM::Environment &env);

	static Individual Mutation(VM::Environment &env, const Individual& ind);

	static Individual Crossover(VM::Environment &env, const Individual& ind1, const Individual& ind2);
	static std::vector<Result> Execute(const std::vector<Individual> &population);

private:
	/// \brief Create individual from program.
	/// \param prog LISP-program.
	Individual(const VM::Program &prog)
	{
	}

	Individual(); /// < Block for contsructor.
};

#endif

