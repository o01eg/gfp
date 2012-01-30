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

#include <fstream>

#include "world_file.h"

bool WorldFile::SetFile(const char* filename)
{
	// if already loaded.
	if(m_FileName == filename)
	{
		return true;
	}

	m_FileName = filename;
	if(m_Map)
	{
		this->~WorldFile();
	}
	std::ifstream f(filename);
	if(f.fail())
	{
		return false;
	}
	f >> m_Width;
	f >> m_Height;
	m_Map = new char*[m_Height];
	for(size_t h = 0; h < m_Height; ++ h)
	{
		m_Map[h] = new char[m_Width];
		for(size_t w = 0; w < m_Width; ++ w)
		{
			f >> m_Map[h][w];
		}
	}
	return true;
}

WorldFile::~WorldFile()
{
	if(m_Map)
	{
		for(size_t h = 0; h < m_Height; ++ h)
		{
			if(m_Map[h])
			{
				delete[] m_Map[h];
			}
		}
		delete[] m_Map;
	}
}

