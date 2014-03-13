SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDATA_DIR=\\\"${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/\\\"")
INCLUDE_DIRECTORIES(vm)

ADD_EXECUTABLE(gfp-labirinth-threaded
	conf.cpp
	conf.h
	functions.cpp
	functions.h
	ga.cpp
	ga.h
	ga_utils.cpp
	ga_utils.h
	gfp-labirinth-threaded.cpp
	individual.cpp
	individual.h
	logbuf.cpp
	logbuf.h
	object_utils.cpp
	object_utils.h
	world.cpp
	world.h
	world_file.cpp
	world_file.h)

ADD_EXECUTABLE(gplisp
	functions.cpp
	functions.h
	gplisp.cpp
	logbuf.cpp
	logbuf.h)
ADD_EXECUTABLE(gfpstatic
	gfpstatic.cpp)
ADD_EXECUTABLE(gpoptimize
	conf.cpp
	conf.h
	functions.cpp
	functions.h
	ga_utils.cpp
	ga_utils.h
	gpoptimize.cpp)

TARGET_LINK_LIBRARIES(gfp-labirinth-threaded vm ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(gplisp vm)
TARGET_LINK_LIBRARIES(gfpstatic vm)
TARGET_LINK_LIBRARIES(gpoptimize vm)

ADD_SUBDIRECTORY(vm)

INSTALL(TARGETS gfp-labirinth-threaded gplisp gfpstatic gpoptimize DESTINATION bin)
