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

# FIXME do not add test sources to non-test target
target_sources(forte-com-opc_ua PRIVATE
        fordiacNamespace.cpp
        forte_datatypetest.cpp
        forte_datatypetest.h
        generated/ua_namespace_fordiacNamespace.c
        generated/ua_namespace_fordiacNamespace.h
        generated/ua_types_fordiacNamespace_generated.c
        generated/ua_types_fordiacNamespace_generated.h
        generated/ua_types_fordiacNamespace_generated_encoding_binary.h
        generated/ua_types_fordiacNamespace_generated_handling.h
)
