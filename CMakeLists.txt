cmake_minimum_required(VERSION 2.6)
project(RoboCInmulator C CXX)


set (CMAKE_CXX_STANDARD 11)
set(CPACK_GENERATOR "TGZ")

set(VREP_FLAGS 
	-DNON_MATLAB_PARSING 
	-DMAX_EXT_API_CONNECTIONS=255 -D__linux
)

set(EXTERN_API 
	remoteApi/extApi.c 
	remoteApi/extApiPlatform.c
)

set(CLASS_SRC
	src/libs/robot_agent.cpp
	src/libs/simulation_group.cpp
)

set(CLASS_HEADERS 
	src/libs/robot_agent.hpp
	src/libs/simulation_group.hpp
)

set(LINK_PARTIAL
	include
	remoteApi
	src/libs
)

include_directories(include)
include_directories(remoteApi)
include_directories(src/libs)

add_executable(simulation ${EXTERN_API} src/simulation_1x1.cpp)
add_executable(test_sim ${EXTERN_API} ${CLASS_SRC} src/tests/test_simulation_group.cpp)
add_executable(test_agent ${EXTERN_API} ${CLASS_SRC} src/tests/test_robotagent.cpp)


target_link_libraries(simulation pthread)
target_compile_options(simulation PUBLIC ${VREP_FLAGS})
set_target_properties(simulation PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(test_sim pthread)
target_compile_options(test_sim PUBLIC ${VREP_FLAGS})
set_target_properties(test_sim PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(test_agent pthread)
target_compile_options(test_agent PUBLIC ${VREP_FLAGS})
set_target_properties(test_agent PROPERTIES LINKER_LANGUAGE CXX)

