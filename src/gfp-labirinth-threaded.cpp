/*
 * Copyright (C) 2014 O01eg <o01eg@yandex.ru> 
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

#include <csignal>
#include <fstream>
#include <ctime>
#include <thread>
//#include <mutex>
#include <atomic>

#include "conf.h"
#include "ga.h"

bool is_run = true;

//std::mutex publish_mutex;
std::atomic_flag publish_flag = ATOMIC_FLAG_INIT;

//data for publish
std::string best_program; //serialized code.
Individual::Result best_result(-1);
size_t total_generations = 0;
time_t st_time;

/// \brief Handler of Ctrl+C event.
/// \param signum Number of signal.
void interrupt_handler(int /*signum*/)
{
	is_run = false;
	VM::Environment::Stop();
}

void threaded_ga(size_t population_size)
{
	GA ga(population_size);
	bool publish = true;
	size_t thread_generation = 0;

	{
		while(publish_flag.test_and_set(std::memory_order_acquire));

		if(! best_program.empty())
		{
			ga.SetInd(0, best_program);
			std::clog << "Load best program: " << best_program << std::endl;
		}

		publish_flag.clear(std::memory_order_release);
	}

	while(is_run)
	{
		if(ga.Step())
		{
			// new best individual
			publish = true;
		}
		
		thread_generation ++;

		if(thread_generation > 500)
		{
			publish = true;
		}

		if(publish)
		{
			if(! publish_flag.test_and_set(std::memory_order_acquire))
			{
				// locked, publish own best individual
				publish = false;

				total_generations += thread_generation;
				thread_generation = 0;

				if(best_result.GetIndex() == -1)
				{
					// It's new here
					best_result = ga.GetInd(0).GetResult();
					best_program = ga.GetInd(0).GetText();
				}
				else
				{
					// compare with previous winner
					if(best_result < ga.GetInd(0).GetResult())
					{
						//best into thread
						ga.SetInd(0, best_program);
					}
					else if(ga.GetInd(0).GetResult() < best_result)
					{
						// thread into best
						best_result = ga.GetInd(0).GetResult();
						best_program = ga.GetInd(0).GetText();

						std::clog << std::endl << "Better." << std::endl;
						/*for(std::vector<Individual>::const_iterator parent = ga.GetInd(0).GetParents().begin(); parent != ga.GetInd(0).GetParents().end(); ++ parent)
						{
							std::clog << "parent = " << parent->GetText() << std::endl;
							parent->GetResult().Dump(std::clog);
							std::clog << "   =====" << std::endl;
						}*/
						std::clog << "1st = " << ga.GetInd(0).GetText() << std::endl;
						ga.GetInd(0).GetResult().Dump(std::clog);
					}
					// otherwise nothing to do.
				}
				
				time_t cur = time(NULL);
				std::clog << "generation = " << total_generations << " (" << static_cast<double>(total_generations)/(cur - st_time) << "g/s)" << std::endl << std::endl;
				std::cout.flush();
				
				publish_flag.clear(std::memory_order_release);
			}
			// otherwise try next generation
		}
	}
}

/// \brief Main Function.
/// \param argc Number of arguments.
/// \param argv Array of arguments.
/// \return Exit code.
int main(int argc, char **argv)
{
	struct sigaction sa;
	sa.sa_handler = interrupt_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	int res = sigaction(SIGINT, &sa, nullptr);
	if(res < 0)
	{
		std::cerr << "Cann't set interrupt handler." << std::endl;
		return 1;
	}

	time_t seed = Config::Instance().GetSLong("seed", time(NULL));
	std::cout << "seed = " << seed << std::endl;
	srand(static_cast<unsigned int>(seed));

	char *filename = NULL;
	if(argc == 2)
	{
		filename = argv[1];
	}

	{
		std::ifstream ifs(filename);
		if(! ifs.fail())
		{
			while(int i = ifs.get())
			{
				if(i == std::char_traits<char>::eof())
				{
					break;
				}
				best_program += static_cast<char>(i);
			}
		}
	}
	
	// Start application.
	std::clog << "Start application" << std::endl;

	size_t num_threads = Config::Instance().GetSLong("threads", time(NULL));
	size_t population_size = Config::Instance().GetSLong("population-size", 10);

	std::vector<std::thread> threads;

	st_time = time(NULL);

	for(size_t t = 0; t < num_threads; ++ t)
	{
		threads.push_back(std::thread(threaded_ga, population_size));
	}

	//while(CurrentState::IsRun())
	//{
		// updating best individual between threads
	//	std::this_thread::yield();
	//}

	for(auto& thread : threads)
	{
		thread.join();
	}

	// save best program to filename
	std::ofstream ofs(filename);
	ofs << best_program;
}

