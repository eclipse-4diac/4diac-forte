#*******************************************************************************
# Copyright (c) 2011 - 2014 nxtControl, ACIN, fortiss GmbH, Profactor GmbH
#               2020 Johannes Kepler University Linz
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Stanislav Meduna, Ingo Hegny, Alois Zoitl, Monika Wenger, Matthias Plasch  - initial API and implementation and/or initial documentation
#    Ernst Blecha - add multibit partial access
# *******************************************************************************/

#SET(SOURCE_GROUP ${SOURCE_GROUP}\\core)

  forte_test_add_inc_directories(${CMAKE_CURRENT_SOURCE_DIR})
  
#BIT-Datatypes  
  forte_test_add_sourcefile_cpp(CIEC_BYTE_PARTIAL_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_WORD_PARTIAL_test.cpp)
  forte_test_add_sourcefile_cpp(CIEC_DWORD_PARTIAL_test.cpp)
IF (FORTE_USE_64BIT_DATATYPES)
  forte_test_add_sourcefile_cpp(CIEC_LWORD_PARTIAL_test.cpp)
ENDIF (FORTE_USE_64BIT_DATATYPES) 

