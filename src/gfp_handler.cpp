#include <glibmm/threadpool.h>
#include "gfp_handler.h"
#include "program.h"

bool operator<(const GFP_Handler::ExamineResult& op1,
	const GFP_Handler::ExamineResult& op2)
{
	/// \todo Write this.
	return false;
}

GFP_Handler::Individual GFP_Handler::Generate()
{
	/// \todo Write this.
	GFP_Handler::Individual result;
	VM::Environment env;
	VM::Object program(env);
	
	return result;
}

std::vector<GFP_Handler::ExamineResult> GFP_Handler::Examine(
	const std::vector<GFP_Handler::Individual> &population)
{
	/// \todo Write this.
	std::vector<GFP_Handler::ExamineResult> res;
	for(size_t i = 0; i < population.size(); i ++)
	{
		res.push_back(GFP_Handler::ExamineResult(i));
	}
	return res;
}

GFP_Handler::Individual GFP_Handler::Mutation(
	const GFP_Handler::Individual &ind)
{
	/// \todo Write this.
	return GFP_Handler::Individual();
}

GFP_Handler::Individual GFP_Handler::Crossover(
	const GFP_Handler::Individual &ind1,
	const GFP_Handler::Individual &ind2)
{
	/// \todo Write this.
	return GFP_Handler::Individual();
}

GFP_Handler::Individual::Individual()
{
}

GFP_Handler::Individual::~Individual()
{
}

