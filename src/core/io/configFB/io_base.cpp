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

#include "io_base.h"

using namespace forte::core::io;

IOConfigFBBase::IOConfigFBBase(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec &paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId) :
    CGenFunctionBlock<CEventSourceFB>(paContainer, paInterfaceSpec, paInstanceNameId) {
}

IOConfigFBBase::~IOConfigFBBase() = default;

bool IOConfigFBBase::createInterfaceSpec(const char *, SFBInterfaceSpec &) {
  // nothing to be done here
  return true;
}

