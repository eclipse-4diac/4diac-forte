#*******************************************************************************
# * Copyright (c) 2019 fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Jose Cabral  - initial API and implementation and/or initial documentation
# *******************************************************************************/

forte_opcua_add_nodeset(fordiacNamespace ${CMAKE_CURRENT_SOURCE_DIR}/generated)

forte_opcua_add_type(forte_datatypetest DataTypeTest "fordiacNamespace") # file name; StructureName; Namespace

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

