cmake_minimum_required(VERSION 2.8)

project (1)

include_directories("/usr/local/include")
include_directories("/usr/local/include/orocos")
link_directories("/usr/local/lib")
link_directories("/usr/local/include")
# add_definitions("-Wall -std=c++14 -llog4cpp -lpthread")
add_definitions("-Wall -std=c++11")
aux_source_directory(src DIR_SRCS)

set(SRC_LIST src/main.cpp
    src/vender/easylogging++.cc
    src/vender/args_converter.cxx
    src/mode1/a.cpp
)

add_subdirectory("test")
add_executable(1 ${SRC_LIST})
target_link_libraries(1 PRIVATE replxx)