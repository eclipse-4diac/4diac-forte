/*******************************************************************************
 * Copyright (c) 2017 - 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "forte/io/configFB/io_adapter_multi.h"
#include "forte/datatypes/forte_bool.h"

namespace forte::io {
  IOConfigFBMultiAdapter::IOConfigFBMultiAdapter(const std::span<const TForteUInt8> paSlaveConfigurationIO,
                                                 CFBContainer &paContainer,
                                                 const SFBInterfaceSpec &paInterfaceSpec,
                                                 const StringId paInstanceNameId,
                                                 TForteUInt8 paParentAdapterlistID) :
      CAdapter(paContainer, paInterfaceSpec, paInstanceNameId, paParentAdapterlistID),
      cmSlaveConfigurationIO(paSlaveConfigurationIO) {
  }

  IOConfigFBMultiAdapter::~IOConfigFBMultiAdapter() = default;

  void IOConfigFBMultiAdapter::setInitialValues() {
    var_QO = false_BOOL;
    var_QI = false_BOOL;
    var_MasterId = 0_UINT;
    var_Index = 0_UINT;
  }
} // namespace forte::io
