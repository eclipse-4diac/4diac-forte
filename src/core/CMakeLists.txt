#*******************************************************************************
# Copyright (c) 2010 - 2016 Profactor GmbH, ACIN, fortiss GmbH
# *                      2018 Johannes Kepler University
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Micheal Hofmann, Alois Zoitl, Gerhard Ebenhofer, Monika Wenger
# *      - initial implementation and rework communication infrastructure
# *    Martin Jobst - adapt for LUA integration
# *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
# *******************************************************************************/
#############################################################################
# Core
#############################################################################
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_subdirectory(cominfra)
forte_add_subdirectory(datatypes)
forte_add_subdirectory(utils)
forte_add_subdirectory(io)
forte_add_subdirectory(fmi)

forte_add_sourcefile_hcpp(conn  dataconn  eventconn)     

forte_add_sourcefile_h   (esfb.h event.h mgmcmd.h fortenode.h fortelist.h genfb.h)
forte_add_sourcefile_hcpp(simplefb basicfb cfb device devexec )
forte_add_sourcefile_hcpp(extevhan funcbloc fbcontainer if2indco)
forte_add_sourcefile_hcpp(resource stringdict typelib ecet)
forte_add_sourcefile_hcpp(adapterconn adapter anyadapter iec61131_functions)

forte_add_sourcefile_with_path_cpp(${CMAKE_BINARY_DIR}/core/deviceExecutionHandlers.cpp) # created file

if(FORTE_DYNAMIC_TYPE_LOAD)
  forte_add_subdirectory(lua)
endif(FORTE_DYNAMIC_TYPE_LOAD)

if(FORTE_SUPPORT_MONITORING)
  forte_add_sourcefile_hcpp(monitoring)
endif(FORTE_SUPPORT_MONITORING)


