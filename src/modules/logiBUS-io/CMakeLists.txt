#*******************************************************************************
# Copyright (c) 2025 HR Agrartechnik GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#     Franz Hoepfinger - initial API and implementation and/or initial documentation
# *******************************************************************************/ 

#############################################################################
# LOGIBUS_IO integration.
#############################################################################

forte_add_io(LOGIBUS "Support for the LOGIBUS_IO (currently only on ESP32")

if(FORTE_IO_LOGIBUS)

add_subdirectory(handle)
add_subdirectory(handler)
add_subdirectory(fb)

endif (FORTE_IO_LOGIBUS) 