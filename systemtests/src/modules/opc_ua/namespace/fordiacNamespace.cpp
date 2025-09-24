/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

#include "forte/com/opc_ua/opcua_nodesets.h"
#include "generated/ua_namespace_fordiacNamespace.h"

namespace forte::com_infra::opc_ua::test {
  namespace {
    [[maybe_unused]] const OPC_UA_Nodesets::EntryImpl<ua_namespace_fordiacNamespace> entry;
  }
} // namespace forte::com_infra::opc_ua::test
