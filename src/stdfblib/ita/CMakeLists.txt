#*******************************************************************************
# Copyright (c) 2010 ACIN
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
# 
# Contributors:
#    Alois Zoitl
# *    - initial API and implementation and/or initial documentation
# *******************************************************************************/
#############################################################################
# Ita FB
#############################################################################
SET(SOURCE_GROUP ${SOURCE_GROUP}\\ita)
forte_add_sourcefile_hcpp(DEV_MGR  EMB_RES  RMT_DEV  RMT_RES)

if(FORTE_SUPPORT_BOOT_FILE)
  forte_add_sourcefile_hcpp(ForteBootFileLoader)
endif(FORTE_SUPPORT_BOOT_FILE)
