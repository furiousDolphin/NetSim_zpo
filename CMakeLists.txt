




cmake_minimum_required( VERSION 3.30 )
project( NetSim )

set( CMAKE_CXX_STANDARD 26 )
#set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules )


include_directories(${PROJECT_SOURCE_DIR}/include)  


file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_library(NetSimLib ${SRC_FILES})


add_executable(NetSimApp main.cpp)

target_link_libraries(NetSimApp NetSimLib)

set_target_properties(NetSimApp PROPERTIES LINK_FLAGS "-Wl,--subsystem,console")


enable_testing()
#add_subdirectory(tests)