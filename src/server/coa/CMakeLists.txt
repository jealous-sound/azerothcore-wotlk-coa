CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} COA_SOURCES)

add_library(coa-interface INTERFACE)
target_include_directories(coa-interface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(coa-interface INTERFACE game-interface)

add_library(coa STATIC ${COA_SOURCES})
target_link_libraries(coa PRIVATE acore-core-interface mysql PUBLIC coa-interface)
set_target_properties(coa PROPERTIES FOLDER "server")

get_target_property(COA_BOOST_INCLUDE_DIRS boost INTERFACE_INCLUDE_DIRECTORIES)
find_path(COA_PROPERTY_TREE_INCLUDE_DIR
  NAMES boost/property_tree/json_parser.hpp
  HINTS ${COA_BOOST_INCLUDE_DIRS}
  NO_DEFAULT_PATH)
if(NOT COA_PROPERTY_TREE_INCLUDE_DIR)
  message(FATAL_ERROR
    "CoA gameplay tests require Boost.PropertyTree headers. Install boost-property-tree for your vcpkg triplet, "
    "or provide a complete Boost installation.")
endif()

foreach(COA_CONFIG coa_bugreport.conf.dist mod_ascension_compat.conf.dist)
  CopyModuleConfig("${CMAKE_CURRENT_SOURCE_DIR}/conf/${COA_CONFIG}" coa)
endforeach()
