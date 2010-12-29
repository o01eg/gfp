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

#include <iostream>
#include <glibmm/thread.h>
#include <csignal>
#include <string>
#include "ga.h"
#include "current_state.h"

/// \brief Maximum step for persistent best individuals.
const size_t MAX_STEP_UNCHANGED = 50000;

/// Handler of Ctrl+D event.
void close_handler()
{
	while(! std::cin.eof())
	{
		char c;
		std::cin >> c;
	}
	CurrentState::Shutdown();
}

/// \brief Handler of Ctrl+C event.
/// \param signum Number of signal.
void interrupt_handler(int signum)
{
	CurrentState::Dump();
	CurrentState::Shutdown();
}

/// \brief Dump individual fitness
/// \param ind Individual.
void dump_individual(const Individual& ind)
{
	std::clog << "result = " << ind.GetResult().m_Result;
	std::clog << "result[DIR_CHANGES] = " << ind.GetResult().m_Quality[Individual::Result::ST_DIR_CHANGES] << std::endl;
	std::clog << "result[MOVE_CHANGES] = " << ind.GetResult().m_Quality[Individual::Result::ST_MOVE_CHANGES] << std::endl;
	std::clog << "result[GOOD_MOVES] = " << ind.GetResult().m_Quality[Individual::Result::ST_GOOD_MOVES] << std::endl;
	std::clog << "result[SUM_MOVES] = " << ind.GetResult().m_Quality[Individual::Result::ST_SUM_MOVES] << std::endl;
	std::clog << "result[ANSWER_QUALITY] = " << ind.GetResult().m_Quality[Individual::Result::ST_ANSWER_QUALITY] << std::endl;
	std::clog << "result[ANSWER_CHANGES] = " << ind.GetResult().m_Quality[Individual::Result::ST_ANSWER_CHANGES] << std::endl;
	std::clog << "result[STATE_CHANGES] = " << ind.GetResult().m_Quality[Individual::Result::ST_STATE_CHANGES] << std::endl;
	std::clog << "result[NEG_CIRCLES] = " << ind.GetResult().m_Quality[Individual::Result::ST_NEG_CIRCLES] << std::endl;
	std::clog << "result[MOVE_DIFF] = " << ind.GetResult().m_Quality[Individual::Result::ST_MOVE_DIFF] << std::endl;
	std::clog << "result[DIR_DIFF] = " << ind.GetResult().m_Quality[Individual::Result::ST_DIR_DIFF] << std::endl;
}

/// \brief Main Function.
/// \param argc Number of arguments.
/// \param argv Array of arguments.
/// \return Exit code.
int main(int argc, char **argv)
{
#if 1
	signal(SIGINT, interrupt_handler);
#endif

	Glib::thread_init();
#if 0
	Glib::Thread::create(sigc::ptr_fun(close_handler), false);
#endif

	time_t seed = time(NULL);
	std::cout << "seed = " << seed << std::endl;
	srand(seed);

	char *filename = NULL;
	if(argc == 2)
	{
		filename = argv[1];
	}
	
	// Start application.
	std::clog << "Start application" << std::endl;
	
	try
	{
#if 0
		VM::Environment env;
		env.LoadFunctions(DATA_DIR "functions.txt");
		try
		{
			VM::Program prog(env, DATA_DIR "2.lsp");
			env.program = &prog;
			const size_t MAX_CIRCLES = 1000;
			VM::Object a(env);
			std::cin >> a;
			env.circle_count = MAX_CIRCLES;
			std::cout << "result = " << env.Run(a) << std::endl;
			std::cout << "use " << (MAX_CIRCLES - env.circle_count) << " circles" << std::endl;
		}
		catch(Glib::Error &e)
		{
			std::clog << "Catch Glib::Error in environment: " << e.what() << std::endl;
		}
#endif
		// Genetic programming
		GA ga(32);
		if(filename)
		{
			try
			{
				ga.Load(filename);
			}
			catch(std::exception &e)
			{
				std::clog << "Catch std::exception: " << e.what() << std::endl;
			}
			catch(Glib::Error &e)
			{
				std::clog << "Catch Glib::Error: " << e.what() << std::endl;
			}
		}
		std::cout << "Start evolution" << std::endl;
		size_t remain_steps = MAX_STEP_UNCHANGED;
		size_t generation = 0;

		while((remain_steps > 0) && CurrentState::IsRun())
		{
			CurrentState::s_Generation = generation;
			if(ga.Step())
			{
				std::clog << "Better." << std::endl;
				remain_steps = MAX_STEP_UNCHANGED;
				std::clog << "1st = " << ga.GetBest().GetText() << std::endl;
				dump_individual(ga.GetBest());
			}
			else
			{
				remain_steps --;
				//std::cout << "steps: " << remain_steps << std::endl;
			}
			generation ++;
			if((generation % 200 == 0) || (remain_steps == MAX_STEP_UNCHANGED))
			{
				std::cout << "generation = " << generation << std::endl;
			}
		}
		std::clog << "1st = " << ga.GetBest().GetText() << std::endl;
		std::clog << "2nd = " << ga.GetBest(1).GetText() << std::endl;
		std::clog << "3nd = " << ga.GetBest(2).GetText() << std::endl;
		std::clog << "4nd = " << ga.GetBest(3).GetText() << std::endl;
		std::clog << "5nd = " << ga.GetBest(4).GetText() << std::endl;
		if(filename)
		{
			ga.Save(filename);
		}

	}
	catch(std::exception &e)
	{
		std::clog << "Catch std::exception: " << e.what() << std::endl;
	}
	catch(Glib::Error &e)
	{
		std::clog << "Catch Glib::Error: " << e.what() << std::endl;
	}
	
	// Finish application.
	std::clog << "Finish application" << std::endl;

	return 0;
}

