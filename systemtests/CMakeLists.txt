if (NOT FORTE_SYSTEM_TESTS)
  return()
endif()

## forte_add_systemtest_hard (test_name bootfile_name timeout)
## Fails if any error has been logged
function(forte_add_systemtest_hard arg1 arg2 arg3)
  file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${arg2}" file_str)
  add_test(NAME ${arg1} COMMAND $<TARGET_FILE:forte> -f "${file_str}")
  set_tests_properties (${arg1} PROPERTIES TIMEOUT ${arg3})
  set_tests_properties(${arg1} PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR: T|==ERROR") #==ERROR is the output when memore leak happens
endfunction()

## forte_add_systemtest_soft (test_name bootfile_name timeout)
## Fails only by TEST_CONDITION FBs. This is for the case when a FB has to be tested in all
## its cases, and some of the cases produce logging error
function(forte_add_systemtest_soft arg1 arg2 arg3)
  file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${arg2}" file_str)
  add_test(NAME ${arg1} COMMAND $<TARGET_FILE:forte> -f "${file_str}")
  set_tests_properties(${arg1} PROPERTIES TIMEOUT ${arg3})
  set_tests_properties(${arg1} PROPERTIES FAIL_REGULAR_EXPRESSION "TEST_CONDITION_FAILED|==ERROR") #==ERROR is the output when memore leak happens
endfunction()

# forte_add_multi_systemtests (TEST_NAME TIMEOUT IS_HARD FILE_NAME EXTRA_ARGS [FILE_NAME EXTRA_ARGS] ...)
function(forte_add_multi_systemtests)
  file(TO_NATIVE_PATH "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/multi_test.cmake" scriptFile)

  list(GET ARGV 0 TEST_NAME)
  list(GET ARGV 1 TEST_TIMEOUT)
  list(GET ARGV 2 TEST_ISHARD)

  set(NEXT_IS_FILE 1)
  set(NEXT_PORT 61499)
  set(COUNTER 0)

  foreach(ARG ${ARGV})
    if (${COUNTER} GREATER 2)

      if (NEXT_IS_FILE)
        file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${ARG}" file_str1)
        list(APPEND TEST_SYS_FILES "${file_str1}")
        set(NEXT_IS_FILE 0)
      else()
        string(FIND "${ARG}" "-c" FLAG_POSITION)
        if(${FLAG_POSITION} EQUAL -1)
          string(APPEND ARG " -c localhost:${NEXT_PORT}")
          math(EXPR NEXT_PORT "${NEXT_PORT}+1")
        endif()

        list(APPEND TEST_EXTRA_ARGS "${ARG}")
        math(EXPR EXTRA_ARG_COUNTER "${EXTRA_ARG_COUNTER}+1")
        set(NEXT_IS_FILE 1)
      endif()

    endif()
    math(EXPR COUNTER "${COUNTER}+1")
  endforeach()

  add_test(NAME ${TEST_NAME} COMMAND ${CMAKE_COMMAND}
    -DCMD=$<TARGET_FILE:forte>
    "-DSYS_FILES=${TEST_SYS_FILES}"
    "-DEXTRA_ARGS=${TEST_EXTRA_ARGS}"
    -P "${scriptFile}")

  set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT ${TEST_TIMEOUT})

  if (${TEST_ISHARD})
    set_tests_properties(${TEST_NAME} PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR: T|==ERROR") #==ERROR is the output when memore leak happens
  else()
    set_tests_properties(${TEST_NAME} PROPERTIES FAIL_REGULAR_EXPRESSION "TEST_CONDITION_FAILED|==ERROR") #==ERROR is the output when memore leak happens
  endif()
endfunction()

function(forte_add_env_file test file)
  file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${file}" file_str)
  set_tests_properties(${test} PROPERTIES ENVIRONMENT "FORTE_BOOT_FILE=${file_str}")
endfunction()

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

forte_add_systemtest_hard(Test_Negating_Connection negatingConnection.fboot 5)

forte_add_systemtest_hard(Test_SEND_RECV_null comSendRecvNull.fboot 5)

if(FORTE_COM_ETH)
  forte_add_systemtest_hard(Test_SEND_RECV_tcp comSendRecvTCP.fboot 30)
  forte_add_systemtest_hard(Test_SEND_RECV_udp comSendRecvUDP.fboot 30)
endif ()

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
STRING(REPLACE "\\" "\\\\" file_str_escaped ${file_str})
set_tests_properties(Test_wrong_bootfile_w_FAIL PROPERTIES ENVIRONMENT "FORTE_BOOT_FILE=${file_str};FORTE_BOOT_FILE_FAIL_MISSING=1")
SET_TESTS_PROPERTIES(Test_wrong_bootfile_w_FAIL PROPERTIES PASS_REGULAR_EXPRESSION "Boot file ${file_str_escaped} could not be opened and FORTE_BOOT_FILE_FAIL_MISSING is set")
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
# disabled when compiling with MSVC until https://github.com/eclipse-4diac/4diac-forte/issues/544 is fixed
if (NOT MSVC)
  forte_add_multi_systemtests(SimpleMultiTest 10 1 multiDevice1.fboot "-c localhost:61500" multiDevice2.fboot " " multiDevice3.fboot "-c localhost:61501")
endif ()

#after compilation, run "make CTEST_OUTPUT_ON_FAILURE=1 test" on the binary folder
