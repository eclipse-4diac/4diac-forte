#*******************************************************************************
# * Copyright (c) 2010 -2105 ACIN, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Alois Zoitl - initial API and implementation and/or initial documentation
# *******************************************************************************/

SET(SOURCE_GROUP ${SOURCE_GROUP}\\phycoreat91)

forte_add_architecture("Phycore AT91")

if("${FORTE_ARCHITECTURE}" STREQUAL "Phycore AT91")

  forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
  
  forte_add_sourcefile_hcpp(../forte_thread ../ecostiha ../forte_sync ../forte_sem)
  forte_add_sourcefile_cpp(phycoreAT91main.cpp ../../genforte_printer.cpp)
  forte_add_sourcefile_h(../../forte_architecture_time.h)

  if(FORTE_COM_ETH)
   forte_add_handler(CFDSelectHandler ../sockhand)
   forte_add_sourcefile_hcpp( ../../fdselecthand ../../bsdsocketinterf)
   forte_add_sourcefile_h(../gensockhand.h)
   forte_add_sourcefile_h(../sockhand.h)
  endif(FORTE_COM_ETH)
  
  forte_add_definition("-Wpointer-arith -Wundef -Woverloaded-virtual -Wno-write-strings")
  forte_add_definition("-W -Wall -Wextra -D_KERNEL -D__ECOS -DFORTE_LITTLE_ENDIAN")
  forte_add_definition("-mcpu=arm7tdmi -msoft-float -mno-thumb-interwork -g -O2")
  forte_add_definition("-ffunction-sections -fdata-sections -fno-threadsafe-statics -fno-rtti -fno-exceptions")


  forte_add_link_flags("-mcpu=arm7tdmi -msoft-float -W,--gc-sections -g -nostartfiles -nostdlib -fno-threadsafe-statics -mno-thumb-interwork -Wl,-static -Wl,-M -Wl,-Map -Wl,forte.map -Wl,-cref")
  
  

endif()