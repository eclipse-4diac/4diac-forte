SET(SOURCE_GROUP ${SOURCE_GROUP}\\systemtests)

if(FORTE_SYSTEM_TESTS)

  forte_add_test(TestShouldNotFail forte_test_1.fboot 5)

  #uncomment the following line to see an example of failing and not failing conditions
  #forte_add_test(TestShouldFail example.fboot 5)
  
  #after compilation, run "make CTEST_OUTPUT_ON_FAILURE=1 test" on the binary folder

endif(FORTE_SYSTEM_TESTS)