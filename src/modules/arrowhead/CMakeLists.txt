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

#############################################################################
# Arrowhead specific functional blocks
#############################################################################

forte_add_module(Arrowhead "Arrowhead Function Blocks")

IF(FORTE_MODULE_Arrowhead)
  add_subdirectory(common)
  add_subdirectory(orchestrator)
  add_subdirectory(serviceRegistry)
  add_subdirectory(SystemTests)
ENDIF(FORTE_MODULE_Arrowhead)



  