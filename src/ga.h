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

#ifndef _GA_H_
#define _GA_H_

#include <algorithm>
#include "individual.h"

/// \class GA
/// \brief Genetic Algoritms.
/// \note operator< for Individual::Result use "less" as better
class GA
{
	typedef std::vector<Individual> Population;
public:
	typedef std::vector<Individual::Result> Results;

	/// \brief Genetic operation.
	/// If first is -1 then it's mutation.
	/// Numbers mean index of parent.
	typedef std::pair<int, int> Operation;
	
	/// \brief GA constructor.
	/// \param population_size_ Size of population.
	GA(size_t population_size_);
	
	~GA() ///< GA Desctructor
	{
		delete m_Population;
	}

	/// \brief Step.
	/// \return True if best individuals changed.
	bool Step();

	/// \brief Examine population.
	/// \return Results.
	Results Examine() const;
	
	/// \brief Save population
	/// \param[in] filename File for saving individual.
	void Save(const char *filename);
	
	/// \brief Load population
	/// \param[in] filename File with saved individual.
	/// \return If file successfully loaded?
	bool Load(const char *filename)
	{
		return Individual::Load(filename, &m_Population->at(m_PopulationSize - 1));
	}

	/// \brief Get individual from population.
	/// Reference actual only before next Step.
	/// \param[in] index Index in population.
	/// \return individual.
	const Individual& GetInd(size_t index) const
	{
		return m_Population->at(index);
	}

	/// \brief Mutation.
	/// \param i Index of individual.
	/// \return Genetic operation.
	static Operation Mutation(int i)
	{
		return Operation(-1, i);
	}

	/// \brief Crossover.
	/// \param i Index of first individual.
	/// \param j Index of second individual.
	/// \return Genetic operation.
	static Operation Crossover(int i, int j)
	{
		return Operation(i, j);
	}

	/// \brief Dump results.
	/// \param[in] population Array of individuals.
	/// \param[in] os Output stream.
	static void DumpResults(const Population& population, std::ostream& os);
private:
	GA(const GA&) = delete; //Prevent copy-constructor.
	GA(GA&&) = delete; //Prevent move-constructor.
	GA& operator=(const GA&) = delete; //Prevent assign.

	size_t m_PopulationSize; ///< Size of population.
	Population *m_Population; ///< Auto pointer to population.
};

#endif
