cmake_minimum_required(VERSION 3.5)
project(md_sse_level1_demo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    include/FixSseApiStruct.hpp
    include/userfunc.hpp
    src/FixSseApiStruct.cpp
    src/userdemo.cpp
    src/userfunc.cpp)

include_directories(.)
include_directories(include)
include_directories(src)

add_executable(md_sse_level1_demo ${SOURCE_FILES})

find_package(Threads REQUIRED)
target_link_libraries(md_sse_level1_demo Threads::Threads)