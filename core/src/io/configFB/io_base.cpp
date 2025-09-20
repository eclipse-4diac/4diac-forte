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

#include "forte/io/configFB/io_base.h"

namespace forte::io {
  IOConfigFBBase::IOConfigFBBase(CFBContainer &paContainer,
                                 const SFBInterfaceSpec &paInterfaceSpec,
                                 const StringId paInstanceNameId) :
      CEventSourceFB(paContainer, paInterfaceSpec, paInstanceNameId) {
  }

  IOConfigFBBase::~IOConfigFBBase() = default;
} // namespace forte::io
