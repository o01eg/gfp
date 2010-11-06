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

#include <fstream>
#include "world_file.h"

void WorldFile::SetFile(const char* filename)
{
	if(m_FileName == filename)
	{
		return;
	}
	m_FileName = filename;
	if(m_Map)
	{
		this->~WorldFile();
	}
	std::ifstream f(filename);
	f >> m_Width;
	f >> m_Height;
	m_Map = new char*[m_Height];
	for(size_t h = 0; h < m_Height; h ++)
	{
		m_Map[h] = new char[m_Width];
		for(size_t w = 0; w < m_Width; w ++)
		{
			f >> m_Map[h][w];
		}
	}
}

WorldFile::~WorldFile()
{
	if(m_Map)
	{
		for(size_t h = 0; h < m_Height; h ++)
		{
			if(m_Map[h])
			{
				delete[] m_Map[h];
			}
		}
		delete[] m_Map;
	}
}

