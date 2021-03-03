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

#############################################################################
# Arrowhead specific functional blocks
#############################################################################

forte_add_module(Arrowhead OFF "Arrowhead Function Blocks")

IF(FORTE_MODULE_Arrowhead)
  add_subdirectory(common)
  add_subdirectory(orchestrator)
  add_subdirectory(serviceRegistry)
  add_subdirectory(eventHandler)
ENDIF(FORTE_MODULE_Arrowhead)



  