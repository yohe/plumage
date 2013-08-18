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
    src/plugin_entity.cpp
    src/plugin_interface.cpp
    src/plugin_information.cpp
    src/plugin_requirement.cpp
    )

add_library(plumage_st STATIC
    src/plugin_manager.cpp
    src/plugin_repository.cpp
    src/plugin_entity.cpp
    src/plugin_interface.cpp
    src/plugin_information.cpp
    src/plugin_requirement.cpp
    )

set(CMAKE_CXX_FLAGS_DEBUG "-g")

make_directory(lib)

set_target_properties(plumage plumage_st PROPERTIES COMPILE_FLAGS "-Wall -I ./include -std=c++11 -stdlib=libc++")

set_target_properties(plumage PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib/")
set_target_properties(plumage_st PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "lib/")
set_target_properties(plumage PROPERTIES LINK_FLAGS "-lc++")
set_target_properties(plumage_st PROPERTIES LINK_FLAGS "-lc++")

INSTALL(TARGETS plumage_st LIBRARY ARCHIVE DESTINATION lib)
INSTALL(TARGETS plumage LIBRARY DESTINATION lib)

INSTALL(FILES
    include/plumage/plugin_entity.hpp 
    include/plumage/plugin_information.hpp
    include/plumage/plugin_interface.hpp
    include/plumage/plugin_manager.hpp
    include/plumage/plugin_repository.hpp
    include/plumage/plugin_requirement.hpp
    include/plumage/plumage_util.hpp
    include/plumage/plumage_config.hpp
    DESTINATION include/plumage)
