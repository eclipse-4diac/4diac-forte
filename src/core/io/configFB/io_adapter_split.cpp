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

#include "io_adapter_split.h"

using namespace forte::core::io;

IOConfigFBSplitAdapter::IOConfigFBSplitAdapter(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec& paInterfaceSpecSocket,
    const CStringDictionary::TStringId paInstanceNameId, const SFBInterfaceSpec& paInterfaceSpecPlug, bool paIsPlug) :
    CAdapter(paContainer, paInterfaceSpecSocket, paInstanceNameId, paInterfaceSpecPlug, paIsPlug) {
}

IOConfigFBSplitAdapter::~IOConfigFBSplitAdapter() = default;

