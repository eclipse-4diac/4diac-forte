#*******************************************************************************
# * Copyright (c) 2014 Profactor GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Matthias Plasch - initial API and implementation and/or initial documentation
# *******************************************************************************/


forte_add_module(RECONFIGURATION "FORTE Online Reconfiguration")

#############################################################################
# FORTE Reconfiguration FBs
#############################################################################
forte_add_sourcefile_hcpp(EC_KILL_ELEM EC_SET_EVT EC_START_ELEM EC_STOP_ELEM)
forte_add_sourcefile_hcpp(ST_CREATE_CONN ST_CREATE_FB ST_DEL_CONN ST_DEL_FB ST_REC_CONN ST_SET_PARM)