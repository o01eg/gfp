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

char **World::s_Map = 0;
size_t World::s_Width = 0;
size_t World::s_Height = 0;

World::World(VM::Environment &env, const char* filename)
{
	if(! s_Map)
	{
		std::ifstream f(filename);
		f >> s_Width;
		f >> s_Height;
		//std::cout << "Width: " << m_Width << " Height: " << m_Height << std::endl;
		s_Map = new char*[s_Height];
		for(size_t h = 0; h < s_Height; h ++)
		{
			s_Map[h] = new char[s_Width];
			for(size_t w = 0; w < s_Width; w ++)
			{
				f >> s_Map[h][w];
			}
		}
	}

	VM::Object map(env);
	for(int hm = s_Height - 1; hm >= 0; hm --)
	{
		VM::Object line(env);
		for(int wm = s_Width - 1; wm >= 0; wm --)
		{
			VM::Object obj(env);
			switch(s_Map[hm][wm])
			{
			case '#':
				obj = VM::Object(VM::Object(env, VM::INTEGER, 0), VM::Object(env));
				break;
			case '@':
				m_PosX = wm;
				m_PosY = hm;
				break;
			}
			line = VM::Object(obj, line);
		}
		map = VM::Object(line, map);
	}
	std::cout << "map: " << map << std::endl;
	std::cout << "x: " << m_PosX << std::endl;
	std::cout << "y: " << m_PosY << std::endl;
}

World::~World()
{
#if 0
	if(m_Map)
	{
		for(size_t h = 0; h < m_Height; h ++)
		{
			delete m_Map[h];
		}
		delete m_Map;
	}
#endif
}

