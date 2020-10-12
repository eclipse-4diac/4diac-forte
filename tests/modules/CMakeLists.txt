#*******************************************************************************
# Copyright (c) 2018 TU Wien/ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Martin Melik Merkumians  - initial API and implementation and/or initial documentation
# *******************************************************************************/
SET(SOURCE_GROUP ${SOURCE_GROUP}\\fblib)

IF(FORTE_MODULE_CONVERT)
  add_subdirectory(convert)
ENDIF()

IF(FORTE_MODULE_IEC61131)
  add_subdirectory(IEC61131-3)
ENDIF()

IF(FORTE_MODULE_UTILS)
  add_subdirectory(utils)
ENDIF()

IF(FORTE_COM_HTTP)
  add_subdirectory(HTTP)
ENDIF()
