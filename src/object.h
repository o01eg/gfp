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

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "weak_object.h"

#if _DEBUG_OBJECT_
#include <set>
#endif

namespace VM
{
	/// \brief LISP Objects.
	class Object : public WeakObject
	{
	public:
		/// \brief Copy constructor.
		/// \param obj Object.
		Object(const Object& obj);

		/// \brief Create NIL object.
		/// \param env Used environment.
		explicit Object(const Environment &env)
			:WeakObject(env)
		{
#if _DEBUG_OBJECT_
			m_AllObjects.insert(this);
#endif			
		}

		/// \brief Create parameterless object.
		/// \param env_ Used environment.
		/// \param type Type of object, may be ERROR, PARAM, QUOTE, IF, EVAL.
		Object(const Environment &env_, Types type);

		/// \brief Create object with one object.
		/// \param env_ Used environment.
		/// \param type Type of object, may be INTEGER, FUNC, ADF.
		/// \param value Value.
		Object(const Environment &env_, Types type, Heap::UInt value);

		/// \brief Create LIST object.
		/// \param head Head object.
		/// \param tail Tail object.
		Object(const Object& head, const Object& tail);

		/// \brief Destructor.
		~Object();

		/// \brief Copying.
		/// \param[in] obj Right value.
		/// \return l-value object.
		Object& operator=(const Object& obj);

		/// \brief Get head of LIST
		/// \return head.
		Object GetHead() const;

		/// \brief Get tail of LIST
		/// \return tail.
		Object GetTail() const;

		/// \brief Get constant environment of object
		/// \return Environment.
		Environment& GetEnv() const {return const_cast<Environment&>(WeakObject::GetEnv());}
#if _DEBUG_OBJECT_
		/// \brief Print all exist objects.
		static void PrintObjects(Environment &env, std::ostream &os);
#endif
	private:
		/// \brief Create object by position.
		/// \sa GetHead GetTail
		/// \param env Environment.
		/// \param pos Position.
		/// \return Generated object.
		static Object GetObjectFrom(Environment &env, Heap::UInt pos);

#if _DEBUG_OBJECT_
		//Object *self; ///< Copy of this (for debugging).
		static std::set<Object*> m_AllObjects; /// < List of all objects.
#endif
	};
}

#endif 

