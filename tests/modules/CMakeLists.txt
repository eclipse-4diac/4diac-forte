#*******************************************************************************
# * Copyright (c) 2018 TU Wien/ACIN
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Martin Melik Merkumians  - initial API and implementation and/or initial documentation
# *******************************************************************************/
SET(SOURCE_GROUP ${SOURCE_GROUP}\\fblib)

IF(FORTE_MODULE_IEC61131)
  add_subdirectory(IEC61131-3)
ENDIF(FORTE_MODULE_IEC61131)

IF(FORTE_MODULE_UTILS)
  add_subdirectory(utils)
ENDIF(FORTE_MODULE_UTILS)
