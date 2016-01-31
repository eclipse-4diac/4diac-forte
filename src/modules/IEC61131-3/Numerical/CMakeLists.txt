#*******************************************************************************
# * Copyright (c) 2011 - 2013 ACIN
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Monika Wenger
# *   - initial API and implementation and/or initial documentation
# *******************************************************************************/
#############################################################################
# Numerical
#############################################################################
forte_add_sourcefile_hcpp(F_ABS)

if(FORTE_USE_REAL_DATATYPE)
	forte_add_sourcefile_hcpp(F_SQRT)
	forte_add_sourcefile_hcpp(F_LN)
	forte_add_sourcefile_hcpp(F_LOG)
	forte_add_sourcefile_hcpp(F_SIN)
	forte_add_sourcefile_hcpp(F_COS)
	forte_add_sourcefile_hcpp(F_TAN)
	forte_add_sourcefile_hcpp(F_ASIN)
	forte_add_sourcefile_hcpp(F_ACOS)
	forte_add_sourcefile_hcpp(F_ATAN)
	forte_add_sourcefile_hcpp(F_EXP)
endif(FORTE_USE_REAL_DATATYPE)