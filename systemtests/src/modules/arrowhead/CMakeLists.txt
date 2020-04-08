# *******************************************************************************
# Copyright (c) 2018, fortiss GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Jose Cabral - initial implementation
# *******************************************************************************

SET(SOURCE_GROUP ${SOURCE_GROUP}\\systemtests)

if(FORTE_SYSTEM_TESTS)

  #test are soft because of the https://github.com/arrowhead-f/core-java/issues/34  

  if(FORTE_COM_HTTP)
    forte_add_systemtest_soft(HTTPArrowheadTests Arrowhead_HTTPTest.fboot 80)
  endif(FORTE_COM_HTTP) 
 
   if(FORTE_COM_OPC_UA)
    forte_add_systemtest_soft(OpcUaArrowheadTests Arrowhead_OpcUaTest.fboot 80)
  endif(FORTE_COM_OPC_UA) 
  
   #after compilation, run "make CTEST_OUTPUT_ON_FAILURE=1 test" on the binary folder

endif(FORTE_SYSTEM_TESTS)