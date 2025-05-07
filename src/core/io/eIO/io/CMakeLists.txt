#*******************************************************************************
# Copyright (c) 2025 Maximilian Scharf
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#   Maximilian Scharf - Initial implementation.
#******************************************************************************/

forte_add_include_directories(.)

# eIO base class
forte_add_sourcefile_h(einputfb.h)
forte_add_sourcefile_h(eConfigFB.h)

# eIO adapter (connecting IO + configuration)
forte_add_sourcefile_hcpp(eGenAdapter_adp)

# eIO FBs
forte_add_sourcefile_hcpp(eIX_fbt)
forte_add_sourcefile_hcpp(eIXconfig_fbt)

forte_add_sourcefile_hcpp(eIW_fbt)
forte_add_sourcefile_hcpp(eIWconfig_fbt)
