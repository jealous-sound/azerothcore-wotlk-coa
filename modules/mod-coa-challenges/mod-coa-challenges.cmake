# Included by modules/CMakeLists.txt. Registers the module's pure helper
# headers AND its host-side unit tests with the core unit_tests target.
set_property(GLOBAL APPEND PROPERTY ACORE_MODULE_TEST_INCLUDES
    "${CMAKE_SOURCE_DIR}/modules/mod-coa-challenges/src")
set_property(GLOBAL APPEND PROPERTY ACORE_MODULE_TEST_SOURCES
    "${CMAKE_SOURCE_DIR}/modules/mod-coa-challenges/tests/CoAChallengeParseTest.cpp")
