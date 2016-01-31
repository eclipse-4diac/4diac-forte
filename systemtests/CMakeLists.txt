
SET(SOURCE_GROUP ${SOURCE_GROUP}\\systemtests)

if(FORTE_SYSTEM_TESTS)

SET(CTEST_OUTPUT_ON_FAILURE TRUE)

forte_add_test(Test1 forte_test_1.fboot 5)
set_tests_properties (Test1 PROPERTIES WILL_FAIL true)
forte_add_test(Test2 forte_test_2.fboot 5)

endif(FORTE_SYSTEM_TESTS)