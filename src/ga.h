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
	size_t m_PopulationSize; ///< Size of population.
	Population *m_Population; ///< Auto pointer to population.
};

#endif
