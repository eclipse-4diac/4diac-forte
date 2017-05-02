#*******************************************************************************
# * Copyright (c) 2010 - 2016 Profactor GmbH, ACIN, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *    Micheal Hofmann, Alois Zoitl, Gerhard Ebenhofer, Monika Wenger
# *      - initial implementation and rework communication infrastructure
# *    Martin Jobst - adapt for LUA integration
# *******************************************************************************/
#############################################################################
# Core
#############################################################################
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_subdirectory(cominfra)
forte_add_subdirectory(datatypes)
forte_add_subdirectory(utils)

forte_add_sourcefile_hcpp(conn  dataconn  eventconn)     

forte_add_sourcefile_h   (esfb.h      event.h    mgmcmd.h      fortenode.h fortelist.h convert_functions.h)
forte_add_sourcefile_hcpp(basicfb     cfb        device      devexec )
forte_add_sourcefile_hcpp(extevhan   funcbloc  fbcontainer  if2indco )
forte_add_sourcefile_hcpp(resource	stringdict typelib       ecet)
forte_add_sourcefile_hcpp(adapterconn adapter    iec61131_functions)

if(FORTE_DYNAMIC_TYPE_LOAD)
	forte_add_subdirectory(lua)
endif(FORTE_DYNAMIC_TYPE_LOAD)

if(FORTE_SUPPORT_MONITORING)
  forte_add_sourcefile_hcpp(monitoring)
endif(FORTE_SUPPORT_MONITORING)


