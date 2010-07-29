#include <iostream>
#include <glibmm/thread.h>
#include <csignal>
#include <string>
#include "ga.h"

/// \brief Maximum step for persistent best individuals.
const size_t MAX_STEP_UNCHANGED = 1;

bool app_is_run = true; ///< Application's state variable.

/// Handler of Ctrl+D event.
void close_handler()
{
	while(! std::cin.eof())
	{
		char c;
		std::cin >> c;
	}
	app_is_run = false;
}

/// \brief Handler of Ctrl+C event.
/// \param signum Number of signal.
void interrupt_handler(int signum)
{
	app_is_run = false;
}

/// \brief Main Function.
/// \param argc Number of arguments.
/// \param argv Array of arguments.
/// \return Exit code.
int main(int argc, char **argv)
{
#if 0
	signal(SIGINT, interrupt_handler);
#endif

	Glib::thread_init();
#if 0
	Glib::Thread::create(sigc::ptr_fun(close_handler), false);
#endif

	srand(time(0));
	
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
		GA ga(10);
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

