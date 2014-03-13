
SET(VM_HEADERS
	environment.h
	heap.h
	ioobject.h
	logger.h
	object.h
	program.h
	weak_object.h)

SET(VM_SRCS
	environment.cpp
	heap.cpp
	ioobject.cpp
	object.cpp
	program.cpp
	weak_object.cpp
)

ADD_LIBRARY(vm ${VM_SRCS} ${VM_HEADERS})
