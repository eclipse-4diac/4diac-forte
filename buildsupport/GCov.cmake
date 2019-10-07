#*******************************************************************************
# Copyright (c) 2014 ACIN and fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Martin Melik-Merkumians, Alois Zoitl - initial API and implementation and/or initial documentation
# *******************************************************************************/

IF(NOT GCOV_PATH)
  FIND_PROGRAM(GCOV_PATH gcov)
  mark_as_advanced(GCOV_PATH)
ENDIF()

IF(NOT LCOV_PATH)
  FIND_PROGRAM(LCOV_PATH lcov)
  mark_as_advanced(LCOV_PATH)
ENDIF()

IF(NOT GENHTML_PATH)
  FIND_PROGRAM(GENHTML_PATH genhtml)
  mark_as_advanced(GENHTML_PATH)
ENDIF()

IF(NOT GCOV_PATH)
  MESSAGE(FATAL_ERROR "Error: gcov not found")
ENDIF()

IF(NOT LCOV_PATH)
  MESSAGE(FATAL_ERROR "Error: lcov not found")
ENDIF()

IF(NOT GENHTML_PATH)
  MESSAGE(FATAL_ERROR "Error:  genhtml not found")
ENDIF()

FUNCTION(SETUP_GCOV targetName testRunner outputName)

        # Setup target
        ADD_CUSTOM_TARGET(${targetName}
                COMMAND ${LCOV_PATH} --directory . --zerocounters
                
                COMMAND ${testRunner} --verbose --log_format=HRF --log_level=test_suite --report_level=no ${ARGV3}
                
                COMMAND ${LCOV_PATH} --directory . --capture --gcov-tool ${GCOV_PATH} --output-file ${outputName}.info --rc lcov_branch_coverage=1
                COMMAND ${LCOV_PATH} --remove ${outputName}.info '*tests*' '/usr/*' '*boost*' --output-file ${outputName}.info.cleaned --rc lcov_branch_coverage=1
                COMMAND ${GENHTML_PATH} -o ${outputName} ${outputName}.info.cleaned --demangle-cpp --rc lcov_branch_coverage=1
                COMMAND ${CMAKE_COMMAND} -E remove ${outputName}.info ${outputName}.info.cleaned
                
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
        
ENDFUNCTION()