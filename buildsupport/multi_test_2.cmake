string(REPLACE " " ";" ARGS_LIST ${ARG1})
string(REPLACE " " ";" ARGS_LIST2 ${ARG2})

list(APPEND ARGS_LIST "-f;${BOOT1}")
list(APPEND ARGS_LIST2 "-f;${BOOT2}")

execute_process(
                COMMAND ${CMD} ${ARGS_LIST}
                COMMAND ${CMD} ${ARGS_LIST2}
                RESULT_VARIABLE result)

if(${result})
  message("TEST_CONDITION_FAILED: The result of the pipe was ${result}")
endif(${result})