#*******************************************************************************
# Copyright (c) 2011 - 2013 ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Monika Wenger, Alois Zoitl
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/


forte_add_module(IEC61131 OFF "IEC61131-3 Function blocks")
#############################################################################
# IEC61131-3 FB
#############################################################################
forte_add_subdirectory(BistableElements)
forte_add_subdirectory(Counters)
forte_add_subdirectory(EdgeDetection)
forte_add_subdirectory(Timers)
forte_add_subdirectory(Conversion)
forte_add_subdirectory(Numerical)
forte_add_subdirectory(BitwiseOperators)
forte_add_subdirectory(Selection)
forte_add_subdirectory(Comparison)
forte_add_subdirectory(Arithmetic)
forte_add_subdirectory(CharacterString)