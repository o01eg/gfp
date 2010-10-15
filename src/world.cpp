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
#include <iostream>
#include "world.h"
#include "ioobject.h"

World::World(VM::Environment &env, const char* filename)
{
	std::ifstream f(filename);
	f >> m_Width;
	f >> m_Height;
	//std::cout << "Width: " << m_Width << " Height: " << m_Height << std::endl;
	m_Map = new char*[m_Height];
	for(size_t h = 0; h < m_Height; h ++)
	{
		m_Map[h] = new char[m_Width];
		for(size_t w = 0; w < m_Width; w ++)
		{
			f >> m_Map[h][w];
		}
	}

	VM::Object map(env);
	for(int hm = m_Height - 1; hm >= 0; hm --)
	{
		VM::Object line(env);
		for(int wm = m_Width - 1; wm >= 0; wm --)
		{
			VM::Object obj(env);
			switch(m_Map[hm][wm])
			{
			case '#':
				obj = VM::Object(VM::Object(env, VM::INTEGER, 0), VM::Object(env));
				break;
			case '@':
				obj = VM::Object(VM::Object(env, VM::INTEGER, -1), VM::Object(env));
				break;
			}
			line = VM::Object(obj, line);
		}
		map = VM::Object(line, map);
	}
	std::cout << "map: " << map << std::endl;
}

World::~World()
{
	if(m_Map)
	{
		for(size_t h = 0; h < m_Height; h ++)
		{
			delete m_Map[h];
		}
		delete m_Map;
	}
}

