#*******************************************************************************
# * Copyright (c) 2011 - 2015 ACIN, fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Alois Zoitl
# *    - initial API and implementation and/or initial documentation
# *******************************************************************************/
#############################################################################
# core/util
#############################################################################
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_sourcefile_h(anyhelper.h staticassert.h singlet.h criticalregion.h)
forte_add_sourcefile_h(fortearray.h fixedcapvector.h)    

forte_add_sourcefile_hcpp(string_utils)
