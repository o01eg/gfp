#ifndef _HEAP_H_
#define _HEAP_H_

#include <vector>
#include "shared.h"

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

		/// \brief Check heap for leaks.
		void CheckLeaks() const;

		/// \brief Allocator.
		/// \param hash
		/// \param value
		/// \param tail
		/// \return Position of allocated object.
		UInt Alloc(UInt hash, UInt value, UInt tail = 0);

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

#endif

