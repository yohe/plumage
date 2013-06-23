cmake_minimum_required(VERSION 2.6)

PROJECT(plumage)

IF(UNIX)
    include_directories(/usr/include ./)
    link_directories(/usr/lib)
ENDIF(UNIX)
IF(APPLE)
    include_directories(/opt/local/include /usr/include ./)
    link_directories(/opt/local/lib /usr/lib)
ENDIF(APPLE)

ADD_LIBRARY(sample_plugin SHARED
    plugin/sample_plugin.cpp
    )

set(CMAKE_CXX_FLAGS_DEBUG "-g")

ADD_EXECUTABLE(plumage_sample sample.cpp)

IF(CPPMODE STREQUAL CXX03)
    add_definitions(-Dnullptr=NULL)
    add_definitions(-DCXX03)
    set_target_properties(sample_plugin plumage_sample PROPERTIES COMPILE_FLAGS "-Wall -I ../include")
ELSE()
    set_target_properties(sample_plugin plumage_sample PROPERTIES COMPILE_FLAGS "-Wall -I ../include")
ENDIF()

set_target_properties(sample_plugin PROPERTIES LINK_FLAGS "-L ../lib -lplumage")
set_target_properties(plumage_sample PROPERTIES LINK_FLAGS "-L ../lib -lplumage")

