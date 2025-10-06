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

#include "forte/cominfra/comlayersmanager.h"

#include "forte/cominfra/comlayer.h"

namespace forte::util::factory {
  template class Factory<DynamicImpl, "", com_infra::CComLayer, com_infra::CComLayer *, com_infra::CBaseCommFB *>;
}
