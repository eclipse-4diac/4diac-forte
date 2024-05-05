#*******************************************************************************
# Copyright (c) 2024 Samator Indo Gas
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Ketut Kumajaya - initial API and implementation and/or initial documentation
# *******************************************************************************/
if(FORTE_WINDOWS_SERVICE)
  forte_add_to_executable_h(ForteService ServiceBase ServiceInstaller ThreadPool)
  forte_add_to_executable_cpp(wmain ForteService ServiceBase ServiceInstaller)
endif()
