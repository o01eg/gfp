#include "heap.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <glibmm/error.h>

int main(int argc, char **argv)
{
	VM::Heap heap;
	const size_t ARR_SIZE = 128;
	const size_t TEST_SIZE = 50;
	size_t ptrs[ARR_SIZE] = { 0 };
	srand(0);

	try
	{
		for(size_t i = 0; i < TEST_SIZE; i ++)
		{
			size_t index = rand() % ARR_SIZE;
			if(ptrs[index])
			{
				std::cout << "= Free " << ptrs[index] << std::endl;
				heap.Detach(ptrs[index]);
				ptrs[index] = 0;
			}
			else
			{
				ptrs[index] = heap.Alloc(0x1, i);
				std::cout << "= Alloc " << ptrs[index] << std::endl;
				if(! ptrs[index])
				{
					std::cout << "Error Alloc" << std::endl;
					break;
				}
			}
		}
		for(size_t index = 0; index < ARR_SIZE; index ++)
		{
			if(ptrs[index])
			{
				heap.Detach(ptrs[index]);
			}
		}
	}
	catch(Glib::Error &e)
	{
		std::cout << "Glib::Error: " << e.what() << std::endl;
	}
	catch(std::exception &e)
	{
		std::cout << "std::exception: " << e.what() << std::endl;
	}

	return 0;
}

