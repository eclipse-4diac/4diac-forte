#*******************************************************************************
# * Copyright (c) 2017 fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Monika Wenger
# *    - initial API and implementation and/or initial documentation
# *******************************************************************************/
#############################################################################
# core/lua
#############################################################################
forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_sourcefile_hcpp(luaengine luatype)
forte_add_sourcefile_hcpp(luabfb luabfbtypeentry)
forte_add_sourcefile_hcpp(luaadapter luaadaptertypeentry)
forte_add_sourcefile_hcpp(luacfb luacfbtypeentry)