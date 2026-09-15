# The runtime gameplay harness uses the header-only Boost.PropertyTree JSON reader.
# Component-based Boost installations (including vcpkg) may omit these headers.
ModuleNameToVariable("mod-ascension-compat" COA_COMPAT_LINKAGE)
if(NOT "${${COA_COMPAT_LINKAGE}}" STREQUAL "disabled")
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
endif()
