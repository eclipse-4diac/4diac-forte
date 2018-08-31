#*******************************************************************************
# * Copyright (c) 2011 - 2018 AIT, ACIN, Profactor GmbH, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Thomas Strasser, Alois Zoitl, Matthias Plasch, Ben Schneider
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/
forte_add_module(UTILS "FORTE UTILITY FBs")

#############################################################################
# FORTE UTILITY FBs
#############################################################################

if(CYGWIN)
  forte_add_definition("-std=gnu++11") 
else(CYGWIN)
  forte_add_definition("-std=c++11")
endif(CYGWIN)

forte_add_sourcefile_hcpp(OUT_ANY_CONSOLE GEN_F_MUX GEN_CSV_WRITER GEN_APPEND_STRING)
forte_add_sourcefile_hcpp(GEN_ARRAY2VALUES GEN_VALUES2ARRAY GEN_ARRAY2ARRAY GET_AT_INDEX SET_AT_INDEX)
forte_add_sourcefile_hcpp(STEST_END)
forte_add_sourcefile_hcpp(FB_RANDOM E_STOPWATCH)

forte_add_subdirectory(tests)
