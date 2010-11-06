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

#include "world.h"
#include "ioobject.h"

WorldFile World::s_File;

World::World(VM::Environment &env, const char *filename)
{
	s_File.SetFile(filename);
	VM::Object map(env);
	for(int hm = s_File.GetHeight() - 1; hm >= 0; hm --)
	{
		VM::Object line(env);
		for(int wm = s_File.GetWidth() - 1; wm >= 0; wm --)
		{
			VM::Object obj(env);
			switch(s_File.GetMap()[hm][wm])
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

