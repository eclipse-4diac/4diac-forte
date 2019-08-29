#*******************************************************************************
# Copyright (c) 2011 - 2014 ACIN, Profactor GmbH
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Monika Wenger, Matthias Plasch
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/
#############################################################################
# BitwiseOperators
#############################################################################
forte_add_sourcefile_hcpp(F_AND)
forte_add_sourcefile_hcpp(F_NOT)
forte_add_sourcefile_hcpp(F_OR)
forte_add_sourcefile_hcpp(F_XOR)
forte_add_sourcefile_hcpp(genbitbase GEN_AND GEN_OR GEN_XOR)

forte_add_sourcefile_hcpp(F_SHL)
forte_add_sourcefile_hcpp(F_SHR)
forte_add_sourcefile_hcpp(F_ROR)
forte_add_sourcefile_hcpp(F_ROL)
