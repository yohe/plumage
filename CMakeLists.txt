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

add_library(plumage SHARED
    src/plugin_manager.cpp
    src/plugin_repository.cpp
    src/plugin_interface.cpp
    )

add_library(plumage_st STATIC
    src/plugin_manager.cpp
    src/plugin_repository.cpp
    src/plugin_interface.cpp
    )

set(CMAKE_CXX_FLAGS_DEBUG "-g")

make_directory(lib)

IF(CPPMODE STREQUAL CXX03)
    add_definitions(-Dnullptr=NULL)
    add_definitions(-DCXX03)
    set_target_properties(plumage plumage_st PROPERTIES COMPILE_FLAGS "-Wall -I ./include")
ELSE()
    set_target_properties(plumage plumage_st PROPERTIES COMPILE_FLAGS "-Wall -I ./include -std=c++11")
ENDIF()

set_target_properties(plumage PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib/")
set_target_properties(plumage_st PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "lib/")

