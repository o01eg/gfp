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

#include <new>
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
	"LIST",
	"EVAL",
	"SYMBOL",
	"MACRO"
};
#endif

using namespace VM;

const Heap::UInt BLOCK_ADDRESS_OFFSET = 8; ///< Number of bits in position pointed to block.
const Heap::UInt BLOCK_SIZE = 1 << BLOCK_ADDRESS_OFFSET; ///< Size of blocks in elements.
const size_t MAX_BLOCKS = 1024;//1 * 1024 * 1024 / (BLOCK_SIZE * sizeof(Heap::UInt)); ///< Maximum blocks count.

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
	bool is_leak = false;
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
				is_leak = true;
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
				is_leak = true;
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
				is_leak = true;
			}
		}
	}
#endif
	if(is_leak)
	{
		THROW("Leaks found.");
	}
}
#endif

Heap::Element& Heap::UnsafeAt(Heap::UInt position, bool allow_free)
{
	Heap::UInt block = position >> BLOCK_ADDRESS_OFFSET;
	Heap::UInt offset = position & (BLOCK_SIZE - 1);
#if _DEBUG_HEAP_
	if(! position)
	{
		THROW(FormatString("Heap 0x", this, ": Request 0 position."));
	}
	if(block >= blocks.size())
	{
		THROW(FormatString("Heap 0x", this, ": Out of array at position ", position, ", block ", block, "."));
	}
	if((! allow_free) && (! blocks[block][offset].hash))
	{
		THROW(FormatString("Heap 0x", this, ": Request free element at position ", position, "."));
	}
#endif
	return blocks[block][offset];
}

const Heap::Element& Heap::UnsafeAt(Heap::UInt position, bool allow_free) const
{
	Heap::UInt block = position >> BLOCK_ADDRESS_OFFSET;
	Heap::UInt offset = position & (BLOCK_SIZE - 1);
#if _DEBUG_HEAP_
	if(! position)
	{
		THROW(FormatString("Heap 0x", this, ": Request 0 position."));
	}
	if(block >= blocks.size())
	{
		THROW(FormatString("Heap 0x", this, ": Out of array at position ", position, ", block ", block, "."));
	}
	if((! allow_free) && (! blocks[block][offset].hash))
	{
		THROW(FormatString("Heap 0x", this, ": Request free element at position ", position, "."));
	}
#endif
	return blocks[block][offset];
}


Heap::UInt Heap::Alloc(Heap::UInt hash, Heap::UInt value, Heap::UInt tail)
{
#if _DEBUG_HEAP_
	if(! hash)
	{
		THROW(FormatString("Heap 0x", this, ": Alloc free element."));
	}
#endif
	if(! blocks[0][0].tail)
	{
		if(blocks.size() >= MAX_BLOCKS)
		{
//#if _DEBUG_HEAP_
			std::cerr << "Heap " << this << ": Reach maximum blocks: " << blocks.size() + 1 << std::endl;
//#endif
			return 0;
		}
		//allocate new block
#if _DEBUG_HEAP_
		std::clog << "Heap " << this << ": Allocate new block. New size is " << blocks.size() + 1 << std::endl;
#endif
		blocks.resize(blocks.size() + 1);
		Heap::UInt last_block = blocks.size() - 1;
		blocks[last_block] = new (std::nothrow) Heap::Element[BLOCK_SIZE];
		if(! blocks[last_block])
		{
#if _DEBUG_HEAP_
			std::clog << "Heap " << this << ": Cann't alloc block." << std::endl;
#endif
			blocks.resize(blocks.size() - 1);
			return 0;
		}
		memset(blocks[last_block], 0, BLOCK_SIZE * sizeof(Heap::Element));
		blocks[0][0].tail = last_block << BLOCK_ADDRESS_OFFSET;
	}
	//have free space
	Heap::UInt pos = blocks[0][0].tail;
	Heap::Element& elem = UnsafeAt(pos, true);
#if _DEBUG_HEAP_
	if(elem.hash)
	{
		THROW(FormatString("Heap 0x", this, ": Trying to allocate used element at position ", pos, "."));
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
#if _DEBUG_HEAP_
		if((elem.tail >> BLOCK_ADDRESS_OFFSET) >= blocks.size())
		{
			THROW(FormatString("Heap 0x", this, ": Try to set free out of array at position ", elem.tail, ", block ", elem.tail >> BLOCK_ADDRESS_OFFSET, "."));
		}
#endif
		blocks[0][0].tail = elem.tail;
	}
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
	if(res)
	{
		UnsafeAt(res, false).at = at;
	}
	return res;
}
#endif

void Heap::Attach(Heap::UInt pos)
{
#if _DEBUG_HEAP_
	if(!pos)
	{
		THROW(FormatString("Heap 0x", this, ": Trying to attach to zero positin"));
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
		THROW(FormatString("Heap 0x", this, ": Trying to detach to zero positin"));
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

