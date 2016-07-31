#*******************************************************************************
# * Copyright (c) 2010 - 2012 ACIN
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
# Net FB
#############################################################################
SET(SOURCE_GROUP ${SOURCE_GROUP}\\net)
forte_add_sourcefile_hcpp(GEN_CLIENT GEN_PUBLISH  GEN_SERVER  GEN_SUBSCRIBE)

if (FORTE_COM_LOCAL) 
  forte_add_sourcefile_hcpp(GEN_PUBL GEN_SUBL) 
endif(FORTE_COM_LOCAL)
