#*******************************************************************************
# * Copyright (c) 2011 - 2014 AIT, ACIN, Profactor GmbH, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Thomas Strasser, Alois Zoitl, Matthias Plasch
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/
forte_add_module(UTILS "FORTE UTILITY FBs")

#############################################################################
# FORTE UTILITY FBs
#############################################################################
forte_add_sourcefile_hcpp(OUT_ANY_CONSOLE GEN_F_MUX GEN_CSV_WRITER GEN_APPEND_STRING)
forte_add_sourcefile_hcpp(GEN_ARRAY2VALUES GEN_VALUES2ARRAY GEN_ARRAY2ARRAY GET_AT_INDEX SET_AT_INDEX)
forte_add_sourcefile_hcpp(STEST_END)

forte_add_subdirectory(tests)
