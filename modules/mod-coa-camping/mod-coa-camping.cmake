if(NOT MODULE_MOD-COA-CAMPING STREQUAL "disabled")
  set_property(GLOBAL APPEND PROPERTY ACORE_MODULE_TEST_INCLUDES
    "${CMAKE_SOURCE_DIR}/modules/mod-coa-camping/src")
  set_property(GLOBAL APPEND PROPERTY ACORE_MODULE_TEST_SOURCES
    "${CMAKE_SOURCE_DIR}/modules/mod-coa-camping/tests/CoACampingRulesTest.cpp")
endif()
