# *******************************************************************************
# Copyright (c) 2011 - 2013 ACIN This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0 which
# is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Monika Wenger, Alois Zoitl *   - initial API and
# implementation and/or initial documentation
# *******************************************************************************/
forte_register_module(IEC61131 OFF "IEC61131-3 Function blocks")
# ############################################################################
# IEC61131-3 FB
# ############################################################################
add_subdirectory(BistableElements)
add_subdirectory(Counters)
add_subdirectory(EdgeDetection)
add_subdirectory(Timers)
add_subdirectory(Conversion)
add_subdirectory(Numerical)
add_subdirectory(BitwiseOperators)
add_subdirectory(Selection)
add_subdirectory(Comparison)
add_subdirectory(Arithmetic)
add_subdirectory(CharacterString)
