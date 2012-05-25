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

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "weak_object.h"

namespace VM
{
	/// \brief LISP Objects.
	class Object : public WeakObject
	{
	public:
		/// \brief Copy constructor.
		/// \param obj Object.
		Object(const Object& obj);

		/// \breif Move constructor.
		/// \param obj Other object.
		Object(Object &&obj);

		/// \brief Create NIL object.
		/// \param env Used environment.
		explicit Object(const Environment &env);

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

		/// \brief Get environment of object.
		/// \warn const_cast must be exists here because WeakObject cann't store mutable or non-const Environment.
		/// \return Environment.
		Environment& GetEnv() const {return const_cast<Environment&>(WeakObject::GetEnv());}
	private:
#if _DEBUG_OBJECT_
		/// \brief Print all exist objects.
		static void PrintObjects(Environment &env, std::ostream &os);
#endif
		/// \brief Create object by position.
		/// \sa GetHead GetTail
		/// \param env Environment.
		/// \param pos Position.
		/// \return Generated object.
		static Object GetObjectFrom(Environment &env, Heap::UInt pos);
	};
}

#endif 

