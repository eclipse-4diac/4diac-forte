set(ARGS_LIST "-f;${BOOT}")

#remove file
file(REMOVE "${CSV_FILE}")

execute_process(COMMAND ${CMD} ${ARGS_LIST}
                RESULT_VARIABLE result)
                
if(${result})
  message("TEST_CONDITION_FAILED | ERROR: T: The result of the pipe was ${result}")
endif(${result})

if(${CSVFILESHOULDEXIST})
  if(NOT EXISTS "${CSV_FILE}")
    message("TEST_CONDITION_FAILED | ERROR: T: The file ${CSV_FILE} should have existed but it's not there\n")
  else(NOT EXISTS "${CSV_FILE}")
    file(READ "${CSV_FILE}" REAL_CONTENT)
    if(NOT "${EXPECTED_CONTENT}" STREQUAL "${REAL_CONTENT}")
      message("TEST_CONDITION_FAILED | ERROR: T: The expected content \n\n${EXPECTED_CONTENT}\n\nis not same as the real one \n\n${REAL_CONTENT}")
    endif(NOT "${EXPECTED_CONTENT}" STREQUAL "${REAL_CONTENT}")
  endif(NOT EXISTS "${CSV_FILE}")
else(${CSVFILESHOULDEXIST})
  if(EXISTS "${CSV_FILE}")
    message("TEST_CONDITION_FAILED | ERROR: T: The file ${CSV_FILE} should have not existed but it's there\n")
  endif(EXISTS "${CSV_FILE}")
endif(${CSVFILESHOULDEXIST})



