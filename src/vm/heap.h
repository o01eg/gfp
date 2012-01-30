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

#ifndef _HEAP_H_
#define _HEAP_H_

#include <vector>
#include "logger.h"

/// \brief Virtual LISP-Machine.
namespace VM
{
	/// \brief Heap.
	class Heap
	{
	public:
		typedef size_t UInt; ///< Unsigned Integer used in heap.

		/// \brief Element of memory.
		struct Element
		{
			/// \brief Hash value.
			/// Zero mean unused element. Less 4 bite mean type.
			UInt hash;

			/// \brief Counter of reference.
			UInt count;

			/// \brief Value or head of list, zero for NIL.
			UInt value;

			/// \brief Tail of list, zero for NIL.
			/// In unused element mean next unused element.
			UInt tail;

#if _DEBUG_HEAP_
			/// \brief Where was allocated.
			const char *at;
#endif
		};
	
		/// \brief Heap constructor.
		Heap();

		/// \brief Heap desctructor.
		/// Check heap for used elements, free them and free allocated system memory.
		~Heap();

		/// \brief Get element from heap.
		/// \param position Index of element.
		/// \return Request element.
		const Element& At(UInt position) const {return UnsafeAt(position, false);}

		/// \brief Get element from heap.
		/// \param position Index of element.
		/// \return Request element.
		const Element& operator[](UInt position) const {return At(position);}

#if _DEBUG_HEAP_		
		/// \brief Check heap for leaks.
		/// \todo Show more information.
		void CheckLeaks(std::ostream &os) const;
#endif

		/// \brief Allocator.
		/// \param hash
		/// \param value
		/// \param tail
		/// \return Position of allocated object.
		UInt Alloc(UInt hash, UInt value, UInt tail = 0);

#if _DEBUG_HEAP_
		/// \brief Debug Allocator
		/// \param at Where was allocated.
		/// \param hash
		/// \param value
		/// \param tail
		/// \return Position of allocated object.
		UInt AllocD(const char *at, UInt hash, UInt value, UInt tail = 0);
#endif

		/// \brief Attacher.
		/// \param pos Position to attached element.
		void Attach(UInt pos);

		/// \brief Detacher and unallocator.
		/// \param pos Position to detached element.
		void Detach(UInt pos);
	private:
		/// \brief Get reference to element.
		/// \param position Index of element.
		/// \param allow_free Allow access to free element.
		/// \return Request element.
		Element& UnsafeAt(UInt position, bool allow_free);

		/// \brief Get reference to element from constant heap.
		/// \param position Index of element.
		/// \param allow_free Allow access to free element.
		/// \return Request element.
		const Element& UnsafeAt(UInt position, bool allow_free) const;

		std::vector<Element*> blocks; ///< Blocks of elements.

		/// \brief Block for copy-constructor.
		/// \param heap Another heap.
		Heap(const Heap& heap);

		/// \brief Block for copying heap.
		/// \param heap Another heap.
		/// \return Reference to heap.
		Heap& operator=(const Heap& heap);
	};
}

#if _DEBUG_HEAP_
 #define Alloc(args...) AllocD(AT, ##args)
#endif

#endif

