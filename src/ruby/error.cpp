#include "error.hpp"
#include "string.hpp"

Ruby::Error::Error(int status)
	:m_Status(status)
{
	VALUE exc_obj = rb_gv_get("$!");
	m_Msg = Ruby::String(rb_obj_as_string(exc_obj)).GetCStr();
	m_Class = Ruby::String(rb_class_path(rb_class_of(exc_obj))).GetCStr();
}

