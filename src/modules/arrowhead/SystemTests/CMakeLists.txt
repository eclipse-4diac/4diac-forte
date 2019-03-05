# *******************************************************************************
# * Copyright (c) 2018, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Jose Cabral - initial implementation
# *******************************************************************************

SET(SOURCE_GROUP ${SOURCE_GROUP}\\systemtests)

if(FORTE_SYSTEM_TESTS)

  #test are soft because of the https://github.com/arrowhead-f/core-java/issues/34  

  if(FORTE_COM_HTTP)
    forte_add_systemtest_soft(HTTPTests Arrowhead_HTTPTest.fboot 80)
  endif(FORTE_COM_HTTP) 
  
   #after compilation, run "make CTEST_OUTPUT_ON_FAILURE=1 test" on the binary folder

endif(FORTE_SYSTEM_TESTS)