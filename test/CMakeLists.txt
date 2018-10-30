cmake_minimum_required(VERSION 2.8)

project (t)

include_directories("/usr/local/include")
include_directories("/usr/local/include/orocos")
include_directories("/usr/local/include/cppunit")
link_directories("/usr/local/lib")
link_directories("/usr/lib")
# add_definitions("-Wall -std=c++14 -llog4cpp -lpthread")
add_definitions("-Wall -std=c++11")
set(SRC_LIST main.cpp
    
    )

add_executable(t ${SRC_LIST})
TARGET_LINK_LIBRARIES(t cppunit)