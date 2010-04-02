#ifndef _GFP_HANDLER_H_
#define _GFP_HANDLER_H_

#include <vector>
#include "environment.h"

/// \brief Handler of Genetic Functional Programming.
class GFP_Handler
{
//public:
	/// \brief Individual.
	class Individual
	{
	public:
		/// \brief Construct new individual;
		Individual();

		/// \brief Destructor. (C.O.)
		~Individual();
	private:
	};
public:	
	/// \brief Examination results.
	class ExamineResult
	{
	public:
		friend bool operator<(const ExamineResult& op1, const ExamineResult& op2);
		
		/// \brief Constructor of result.
		/// \param index_ Index of individual in population.
		ExamineResult(size_t index_):index(index_) {}
		
		/// \brief Return index of individual.
		/// \return Index.
		size_t GetIndex()
		{
			return index;
		}
	private:
		size_t index; ///< Index of individual in population.
	};
	
	/// \brief Generate new random individaul.
	/// \return New individual.
	Individual Generate();
	
	/// \brief Mutation.
	/// \param ind Parent.
	/// \return Child.
	Individual Mutation(const Individual &ind);
	
	/// \brief Crossover.
	/// \param ind1 First parent.
	/// \param ind2 Second parent.
	/// \return Child.
	Individual Crossover(const Individual &ind1,const Individual &ind2);
	
	/// \brief Examine population.
	/// \param population Population.
	/// \return Results of examination.
	std::vector<ExamineResult> Examine(const std::vector<Individual> &population);
private:
};

/// \brief Compare results of individuals.
/// \param op1 Left operand.
/// \param op2 Right operand.
/// \return Result of comparing
bool operator<(const GFP_Handler::ExamineResult& op1, const GFP_Handler::ExamineResult& op2);

#endif

