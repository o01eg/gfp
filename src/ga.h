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
	typedef std::vector<Individual::Result> Results;
public:
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
	/// \param operations Rules of creating new generation.
	/// \return True if best individuals changed.
	bool Step(const std::vector<Operation> &operations);
	
	/// \brief Save population
	void Save()
	{
		/// \todo Write this.
	}
	
	/// \brief Load population
	void Load()
	{
		/// \todo Write this.
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
private:
	size_t m_PopulationSize; ///< Size of population.
	Population *m_Population; ///< Auto pointer to population.
};

#endif
