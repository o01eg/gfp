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

#if _DEBUG_HEAP_
#include <iostream>
#endif
#include <string.h>
#include "heap.h"

#undef Alloc

#if _DEBUG_HEAP_
const char *type_str[] = {
	"ERROR",
	"INTEGER",
	"FUNC",
	"ADF",
	"PARAM",
	"QUOTE",
	"IF",
	"LIST"
};
#endif

using namespace VM;

const Heap::UInt BLOCK_ADDRESS_OFFSET = 4; ///< Number of bits in position pointed to block.
const Heap::UInt BLOCK_SIZE = 1 << BLOCK_ADDRESS_OFFSET; ///< Size of blocks in elements.

Heap::Heap()
	:blocks(1)
{
	blocks[0] = new Heap::Element[BLOCK_SIZE];
	memset(blocks[0], 0, BLOCK_SIZE * sizeof(Heap::Element));
	blocks[0][0].tail = 1;
#if _DEBUG_HEAP_
	std::clog << "Heap " << this << ": Created." << std::endl;
#endif
}

Heap::~Heap()
{
#if _DEBUG_HEAP_
	CheckLeaks(std::clog);
#endif
	for(Heap::UInt i = 0; i < blocks.size(); i ++)
	{
		delete[] blocks[i];
	}
#if _DEBUG_HEAP_
	std::clog << "Heap " << this << ": Destroyed." << std::endl;
#endif
}

#if _DEBUG_HEAP_
void Heap::CheckLeaks(std::ostream &os) const
{
	Heap::UInt i, j;
#if _DOT_MEMORY_
	for(i = 0; i < blocks.size(); i ++)
	{
		for(j = 0 ; j < BLOCK_SIZE; j ++)
		{
			Heap::UInt pos = ((i << BLOCK_ADDRESS_OFFSET) | j);
			if(blocks[i][j].hash && ((blocks[i][j].hash & 0xf) == 7))
			{
				os << pos << " [shape = record, label = \"" << pos << "|<head>|<tail>\"];" << std::endl;
				if(blocks[i][j].value)
				{
					os << pos << ":head -> " << blocks[i][j].value << ";" << std::endl;
				}
				if(blocks[i][j].tail)
				{
					os << pos << ":tail -> " << blocks[i][j].tail << ";" << std::endl;
				}
			}
		}
	}
	for(i = 0; i < blocks.size(); i ++)
	{
		for(j = 0 ; j < BLOCK_SIZE; j ++)
		{
			Heap::UInt pos = ((i << BLOCK_ADDRESS_OFFSET) | j);
			if(blocks[i][j].hash && ((blocks[i][j].hash & 0xf) != 7))
			{
				os << pos << " [shape = record, label = \"" << pos << " " << type_str[blocks[i][j].hash & 0xf] << ":" << blocks[i][j].value << "\"];" << std::endl;
			}
		}
	}
#else
	os << "Heap: " << this << ": Free pointer: " << blocks[0][0].tail << std::endl;
	for(i = 0; i < blocks.size(); i ++)
	{
		for(j = 0 ; j < BLOCK_SIZE; j ++)
		{
			if(blocks[i][j].hash)
			{
				/// \todo Add here call for logging leaks.
				os << "Heap " << this << ": Leak at position " << ((i << BLOCK_ADDRESS_OFFSET) | j);
				os << " type:" << type_str[blocks[i][j].hash & 0xf];
				os << " count:" << blocks[i][j].count;
				os << " value:" << blocks[i][j].value;
				os << " tail:" << blocks[i][j].tail;
				os << " " << blocks[i][j].at << std::endl;
			}
		}
	}
#endif
}
#endif

Heap::Element& Heap::UnsafeAt(Heap::UInt position, bool allow_free)
{
	Heap::UInt block = position >> BLOCK_ADDRESS_OFFSET;
	Heap::UInt offset = position & (BLOCK_SIZE - 1);
#if _DEBUG_HEAP_
	if(! position)
	{
		THROW(Glib::ustring::compose("Heap 0x%1: Request 0 position.", this));
	}
	if(block >= blocks.size())
	{
		THROW(Glib::ustring::compose("Heap 0x%1: Out of array at position %2, block %3.", this, position, block));
	}
	if((! allow_free) && (! blocks[block][offset].hash))
	{
		THROW(Glib::ustring::compose("Heap 0x%1: Request free element at position %1.", this, position));
	}
#endif
	return blocks[block][offset];
}

const Heap::Element& Heap::UnsafeAt(Heap::UInt position, bool allow_free) const
{
	Heap::UInt block = position >> BLOCK_ADDRESS_OFFSET;
	Heap::UInt offset = position & (BLOCK_SIZE - 1);
#if _DEBUG_HEAP_
	if( !position)
	{
		THROW(Glib::ustring::compose("Heap 0x%1: Request 0 position.", this));
	}
	if(block >= blocks.size())
	{
		THROW(Glib::ustring::compose("Heap 0x%1: Out of array at position %2, block %3.", this, position, block));
	}
	if((! allow_free) && (! blocks[block][offset].hash))
	{
		THROW(Glib::ustring::compose("Heap 0x%1: Request free element at position %2.", this, position));
	}
#endif
	return blocks[block][offset];
}


Heap::UInt Heap::Alloc(Heap::UInt hash, Heap::UInt value, Heap::UInt tail)
{
#if _DEBUG_HEAP_
	if(! hash)
	{
		THROW(Glib::ustring::compose("Heap 0x%1: Alloc free element.", this));
	}
#endif
	if(! blocks[0][0].tail)
	{
		//allocate new block
#if _DEBUG_HEAP_
		std::clog << "Heap " << this << ": Allocate new block. New size is " << blocks.size() + 1 << std::endl;
#endif
		blocks.resize(blocks.size() + 1);
		Heap::UInt last_block = blocks.size() - 1;
		blocks[last_block] = new Heap::Element[BLOCK_SIZE];
		memset(blocks[last_block], 0, BLOCK_SIZE * sizeof(Heap::Element));
		blocks[0][0].tail = last_block << BLOCK_ADDRESS_OFFSET;
	}
	//have free space
	Heap::UInt pos = blocks[0][0].tail;
	Heap::Element& elem = UnsafeAt(pos, true);
#if _DEBUG_HEAP_
	if(elem.hash)
	{
		THROW(Glib::ustring::compose("Heap 0x%1: Trying to allocate used element at position %2.", this, pos));
	}
#endif
	// element is free. Tail point to other free element.
	if((! elem.tail) && (pos < ((blocks.size() << BLOCK_ADDRESS_OFFSET) - 1)) && (! UnsafeAt(pos + 1, true).hash))
	{
		// elem.tail point to null and allocated position before end and next element is free
		blocks[0][0].tail = pos + 1;
	}
	else
	{
		blocks[0][0].tail = elem.tail;
	}
#if 0 // not need now because if check after
	if(blocks[0][0].tail && UnsafeAt(blocks[0][0].tail, true).hash) // Check new point of free pointer.
	{
		THROW(Glib::ustring::compose("Heap 0x%1: Free pointer point to used element at position %2.", this, pos + 1));
	}
#endif
	elem.hash = hash;
	elem.count = 1;
	elem.value = value;
	elem.tail = tail;
	return pos;
}

#if _DEBUG_HEAP_
Heap::UInt Heap::AllocD(const char *at, UInt hash, UInt value, UInt tail)
{
	UInt res = Alloc(hash, value, tail);
	UnsafeAt(res, false).at = at;
	return res;
}
#endif

void Heap::Attach(Heap::UInt pos)
{
#if _DEBUG_HEAP_
	if(!pos)
	{
		THROW(Glib::ustring::compose("Heap 0x%1: Trying to attach to zero positin", this));
	}
#endif
	Heap::Element& elem = UnsafeAt(pos, false);
	elem.count ++;
}

void Heap::Detach(Heap::UInt pos)
{
#if _DEBUG_HEAP_
	if(! pos)
	{
		THROW(Glib::ustring::compose("Heap 0x%1: Trying to detach to zero positin", this));
	}
#endif
	Heap::Element& elem = UnsafeAt(pos, false);
	elem.count --;
	if(! elem.count)
	{
		//unallocate element
		elem.tail = blocks[0][0].tail;
		elem.hash = 0;
		blocks[0][0].tail = pos;
	}
}

