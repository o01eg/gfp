#ifndef _STACK_H_
#define _STACK_H_

#include <stack>
#include "shared.h"
#include "heap.h"

namespace VM
{
	/// \class Stack.
	/// \todo Write this.
	class Stack
	{
		public:
			Stack() {} ///< Contructor of stack.
			~Stack() {} ///< Destructor of stack.
		private:
			Heap::UInt *data; ///< Stored data.
			
	};
}

#endif

