#*******************************************************************************
# Copyright (c) 2010 - 2012 ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Alois Zoitl
# *    - initial API and implementation and/or initial documentation
# *******************************************************************************/
#############################################################################
# Net FB
#############################################################################
SET(SOURCE_GROUP ${SOURCE_GROUP}\\net)
forte_add_sourcefile_hcpp(GEN_CLIENT GEN_PUBLISH  GEN_SERVER  GEN_SUBSCRIBE)

forte_add_sourcefile_hcpp(PUBLISH_1)
forte_add_sourcefile_hcpp(SUBSCRIBE_1)

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (FORTE_COM_LOCAL) 
  forte_add_sourcefile_hcpp(GEN_PUBL GEN_SUBL) 
endif(FORTE_COM_LOCAL)
