SET(SOURCE_GROUP ${SOURCE_GROUP}\\systemtests)

if(FORTE_SYSTEM_TESTS)

  add_subdirectory(src)

  #hard_pass_example.fboot doesn't produce any logging error and all TEST_CONDITION FBs pass
  #soft_pass_example.fboot produces logging error but all TEST_CONDITION FBs pass
  #fail_example.fboot has TEST_CONDITION FBs that fail, so it always fail, no matter hard or soft

  forte_add_systemtest_hard(Test_Hard_Pass hard_pass_example.fboot 5)
  
  forte_add_systemtest_soft(Test_Soft_Pass_Clean hard_pass_example.fboot 5)
  forte_add_systemtest_soft(Test_Soft_Pass_Dirty soft_pass_example.fboot 5)
  forte_add_systemtest_hard(Test_DeInitCommFB deInit_commFb.fboot 5) #check that the de-initialization of a CommFB produce an INITO
  
  forte_add_systemtest_hard(Test_Soft_Example_As_Hard_Inverted soft_pass_example.fboot 5)
  SET_TESTS_PROPERTIES(Test_Soft_Example_As_Hard_Inverted PROPERTIES WILL_FAIL TRUE)
  forte_add_systemtest_hard(Test_Hard_Fail_Alaways_Inverted fail_example.fboot 5)
  SET_TESTS_PROPERTIES(Test_Hard_Fail_Alaways_Inverted PROPERTIES WILL_FAIL TRUE)
  forte_add_systemtest_soft(Test_Soft_Fail_Alaways_Inverted fail_example.fboot 5)
  SET_TESTS_PROPERTIES(Test_Soft_Fail_Alaways_Inverted PROPERTIES WILL_FAIL TRUE)
  
  #Test using environement variable as bootfile
  ##############################################
  ADD_TEST(NAME Test_Hard_Pass_ENV COMMAND $<TARGET_FILE:forte>)
  set_tests_properties (Test_Hard_Pass_ENV PROPERTIES TIMEOUT 5)
  forte_add_env_file(Test_Hard_Pass_ENV hard_pass_example.fboot)
  SET_TESTS_PROPERTIES(Test_Hard_Pass_ENV PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR: T")
  ##############################################
  
  #Test using local bootfile
  ##############################################
  FILE(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/hard_pass_example.fboot" file_str)
  STRING(REPLACE "\\" "\\\\" file_str ${file_str})
  configure_file(${file_str} ${CMAKE_CURRENT_BINARY_DIR}/forte.fboot COPYONLY)
  ADD_TEST(NAME Test_Hard_Pass_LOCAL COMMAND $<TARGET_FILE:forte>)
  set_tests_properties (Test_Hard_Pass_LOCAL PROPERTIES TIMEOUT 5)
  SET_TESTS_PROPERTIES(Test_Hard_Pass_LOCAL PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR: T")
  ##############################################
  
  #Test bootfile with a missing semicolon
  ##############################################
  ADD_TEST(NAME Test_missing_semicolon COMMAND $<TARGET_FILE:forte>)
  set_tests_properties (Test_missing_semicolon PROPERTIES TIMEOUT 5)
  forte_add_env_file(Test_missing_semicolon missing_semicolon.fboot)
  SET_TESTS_PROPERTIES(Test_missing_semicolon PROPERTIES PASS_REGULAR_EXPRESSION "Boot file line does not contain separating ';'")
  ##############################################
  
  #Test unkonwn FB
  ##############################################
  ADD_TEST(NAME Test_missing_unknown_FB COMMAND $<TARGET_FILE:forte>)
  set_tests_properties (Test_missing_unknown_FB PROPERTIES TIMEOUT 5)
  forte_add_env_file(Test_missing_unknown_FB unknown_FB.fboot)
  SET_TESTS_PROPERTIES(Test_missing_unknown_FB PROPERTIES PASS_REGULAR_EXPRESSION "Boot file command could not be executed")
  ##############################################
  
  #Test wrong boot-file with FORTE_BOOT_FILE_FAIL_MISSING set
  ##############################################
  ADD_TEST(NAME Test_wrong_bootfile_w_FAIL COMMAND $<TARGET_FILE:forte>)
  set_tests_properties (Test_wrong_bootfile_w_FAIL PROPERTIES TIMEOUT 5)
  FILE(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/non_existing.fboot" file_str)
  STRING(REPLACE "\\" "\\\\" file_str ${file_str})
  set_tests_properties(Test_wrong_bootfile_w_FAIL PROPERTIES ENVIRONMENT "FORTE_BOOT_FILE=${file_str};FORTE_BOOT_FILE_FAIL_MISSING=1")
  SET_TESTS_PROPERTIES(Test_wrong_bootfile_w_FAIL PROPERTIES PASS_REGULAR_EXPRESSION "Boot file ${file_str} could not be opened and FORTE_BOOT_FILE_FAIL_MISSING is set")
  ##############################################
  
  #Test help parameter (-h)
  ##############################################
  ADD_TEST(NAME Test_help_parameter COMMAND $<TARGET_FILE:forte> -h)
  set_tests_properties (Test_help_parameter PROPERTIES TIMEOUT 5)
  SET_TESTS_PROPERTIES(Test_help_parameter PROPERTIES PASS_REGULAR_EXPRESSION "Usage: forte \\[options\\]")
  ##############################################
  
  #Test wrong parameter (missing -)
  ##############################################
  ADD_TEST(NAME Test_wrong_parameter COMMAND $<TARGET_FILE:forte> f file1)
  set_tests_properties (Test_wrong_parameter PROPERTIES TIMEOUT 5)
  SET_TESTS_PROPERTIES(Test_wrong_parameter PROPERTIES PASS_REGULAR_EXPRESSION "Usage: forte \\[options\\]")
  ##############################################
  
  #Test invalid parameter (-z)
  ##############################################
  ADD_TEST(NAME Test_invalid_parameter COMMAND $<TARGET_FILE:forte> -z)
  set_tests_properties (Test_invalid_parameter PROPERTIES TIMEOUT 5)
  SET_TESTS_PROPERTIES(Test_invalid_parameter PROPERTIES PASS_REGULAR_EXPRESSION "Usage: forte \\[options\\]")
  ##############################################
  
  #Test valid and invalid parameter (-c localhost:61499 -z)
  ##############################################
  ADD_TEST(NAME Test_valid_invalid_parameter COMMAND $<TARGET_FILE:forte> -c localhost:61499 -z)
  set_tests_properties (Test_valid_invalid_parameter PROPERTIES TIMEOUT 5)
  SET_TESTS_PROPERTIES(Test_valid_invalid_parameter PROPERTIES PASS_REGULAR_EXPRESSION "Usage: forte \\[options\\]")
  ##############################################
  
  # Simple test with multi devices
  forte_add_multi_systemtests(SimpleMultiTest 10 1 multiDevice1.fboot "-c localhost:61500" multiDevice2.fboot " " multiDevice3.fboot "-c localhost:61501")
  
  #after compilation, run "make CTEST_OUTPUT_ON_FAILURE=1 test" on the binary folder

endif(FORTE_SYSTEM_TESTS)