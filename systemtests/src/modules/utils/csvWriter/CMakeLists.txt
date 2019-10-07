#*******************************************************************************
# Copyright (c) 2019 fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Jose Cabral  - initial API and implementation and/or initial documentation
# *******************************************************************************/

#############################################################################
# Tests for the the csv writer FBs
#############################################################################

FUNCTION(ADD_CSVWRITE_TEST test_name bootFile csvFileName csvFileShouldExist expectedContent isHard timeout)

  FILE(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/csvWritertTest.cmake" scriptFile)
  STRING(REPLACE "\\" "\\\\" scriptFile ${scriptFile})
  
  FILE(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${bootFile}" file_str)
  STRING(REPLACE "\\" "\\\\" file_str ${file_str})

  ADD_TEST(NAME ${test_name} COMMAND ${CMAKE_COMMAND}
         -DCMD=$<TARGET_FILE:forte>
         -DBOOT=${file_str}
         -DCSV_FILE=${csvFileName}
         -DCSVFILESHOULDEXIST=${csvFileShouldExist}
         -DEXPECTED_CONTENT=${expectedContent}
         -P ${scriptFile})
  
  set_tests_properties (${test_name} PROPERTIES TIMEOUT ${timeout})
  
  if(${isHard})
    SET_TESTS_PROPERTIES(${test_name} PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR: T") 
  else()
    SET_TESTS_PROPERTIES(${test_name} PROPERTIES FAIL_REGULAR_EXPRESSION "TEST_CONDITION_FAILED") 
  endif()
  
ENDFUNCTION(ADD_CSVWRITE_TEST)

ADD_CSVWRITE_TEST(Test_CSV_Writer_normalExecution_hard normalExecution.fboot "csvWriteTest.csv" 1 "'csvFirstValue'\; \n" 1 5)
ADD_CSVWRITE_TEST(Test_CSV_Writer_openAndNoClose_hard openAndNoClose.fboot "csvWriteTest.csv" 1 "" 1 5)
ADD_CSVWRITE_TEST(Test_CSV_Writer_doubleInit_soft doubleInit.fboot "csvWriteTest.csv" 1 "" 0 5)
ADD_CSVWRITE_TEST(Test_CSV_Writer_initDeinitNoWrite_hard initDeinitNoWrite.fboot "csvWriteTest.csv" 1 "" 1 5)
ADD_CSVWRITE_TEST(Test_CSV_Writer_closeNotOpened_soft closeNotOpened.fboot "csvWriteTest.csv" 0 "" 0 5)
ADD_CSVWRITE_TEST(Test_CSV_Writer_writeQIFalse_soft writeQIFalse.fboot "csvWriteTest.csv" 1 "" 0 5)
ADD_CSVWRITE_TEST(Test_CSV_Writer_writeWithoutInit_soft writeWithoutInit.fboot "csvWriteTest.csv" 0 "" 0 5)

ADD_CSVWRITE_TEST(Test_CSV_Writer_normalExecutionMultiple_hard normalExecutionMultiple.fboot "csvWriteTest.csv" 1 "'csvFirstValue'\; TRUE\; 1\; \n'csvFirstValue'\; FALSE\; 2\; \n" 1 5)

ADD_CSVWRITE_TEST(Test_CSV_Writer_noNameFile_soft writeWithoutInit.fboot "noNameFile.csv" 0 "" 0 5)



