/*
 * Copyright (C) 2010-2011 O01eg <o01eg@yandex.ru> 
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

#include "scheme.h"

SCM ScmPrimitiveLoad(void *data);
SCM ScmHandle(void *data, SCM key, SCM parameter);

SCM ScmPrimitiveLoad(void *data)
{
	return scm_c_primitive_load(reinterpret_cast<const char*>(data));
}

SCM ScmHandle(void *data, SCM key, SCM parameter)
{
	return SCM_BOOL_T;
}

Scheme::Scheme()
{
	scm_init_guile();
}

Scheme::~Scheme()
{
}

SCM Scheme::PrimitiveLoad(const char *filename)
{
	SCM scm = scm_c_catch(SCM_BOOL_T, ScmPrimitiveLoad, const_cast<void*>(reinterpret_cast<const void*>(filename)), ScmHandle, NULL, NULL, NULL);
	return scm;
}


