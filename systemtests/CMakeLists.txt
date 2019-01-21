SET(SOURCE_GROUP ${SOURCE_GROUP}\\systemtests)

if(FORTE_SYSTEM_TESTS)

  #hard_pass_example.fboot doesn't produce any logging error and all TEST_CONDITION FBs pass
  #soft_pass_example.fboot produces logging error but all TEST_CONDITION FBs pass
  #fail_example.fboot has TEST_CONDITION FBs that fail, so it always fail, no matter hard or soft

  forte_add_systemtest_hard(Test_Hard_Pass hard_pass_example.fboot 5)
  
  forte_add_systemtest_soft(Test_Soft_Pass_Clean hard_pass_example.fboot 5)
  forte_add_systemtest_soft(Test_Soft_Pass_Dirty soft_pass_example.fboot 5) 
  
  #uncomment the following  lines to see examples of failing conditions
  
  #forte_add_systemtest_hard(Test_Soft_Example_As_Hard soft_pass_example.fboot 5)
  #forte_add_systemtest_hard(Test_Hard_Fail_Alaways fail_example.fboot 5)
  #forte_add_systemtest_soft(Test_Soft_Fail_Alaways fail_example.fboot 5)
  
  #after compilation, run "make CTEST_OUTPUT_ON_FAILURE=1 test" on the binary folder

endif(FORTE_SYSTEM_TESTS)