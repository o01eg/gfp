#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "environment.h"

namespace VM
{
	/// \brief LISP Objects.
	class Object
	{
	public:
		/// \brief Types of objects.
		enum Types
		{
			ERROR = 0,
			INTEGER,
			FUNC,
			ADF,
			PARAM,
			QUOTE,
			IF,
			LIST/*,
			NIL = -1 */
		};
		/// \brief Copy constructor.
		/// \param obj Object.
		Object(const Object& obj);

		/// \brief Create NIL object.
		/// \param env_ Used environment.
		Object(Environment &env_)
			:env(env_), pos(0) {}

		/// \brief Create parameterless object.
		/// \param env_ Used environment.
		/// \param type Type of object, may be ERROR, PARAM, QUOTE, IF.
		Object(Environment &env_, Types type);

		/// \brief Create object with one object.
		/// \param env_ Used environment.
		/// \param type Type of object, may be INTEGER, FUNC, ADF.
		/// \param value Value.
		Object(Environment &env_, Types type, Heap::UInt value);

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

		/// \brief Compare.
		/// \param[in] obj Right value.
		/// \return result of comparsion.
		bool operator==(const Object& obj) const;

		/// \brief Check object for NIL.
		/// \return Answer.
		bool IsNIL() const {return (! pos);}

		/// \brief Get type.
		/// You must check object about NIL before call this function.
		/// \sa IsNIL
		/// \return type of object.
		Types GetType() const {return static_cast<Types>(env.heap.At(pos).hash & 0xf);}

		/// \brief Get value.
		/// You must check object about NIL before call this function.
		/// \sa IsNIL
		/// \return Value of object.
		Heap::UInt GetValue() const;

		/// \brief Get head of LIST
		/// \return head.
		Object GetHead() const;

		/// \brief Get tail of LIST
		/// \return tail.
		Object GetTail() const;

		/// \brief Get constant environment of object
		/// \return Environment.
		Environment& GetEnv() const {return const_cast<Environment&>(env);}
	private:
		/// \brief Create object by position.
		/// \sa GetHead GetTail
		/// \param env Environment.
		/// \param pos Position.
		/// \return Generated object.
		static Object GetObjectFrom(Environment &env, Heap::UInt pos);

		Environment &env; ///< Current environment.
		Heap::UInt pos; ///< Position in heap.
	};
}

#endif 

