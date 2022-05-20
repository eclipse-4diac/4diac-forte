/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "E_MERGE.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_MERGE_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(E_MERGE, g_nStringIdE_MERGE)

const CStringDictionary::TStringId E_MERGE::scm_anEventInputNames[] = {g_nStringIdEI1, g_nStringIdEI2};

const CStringDictionary::TStringId E_MERGE::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec E_MERGE::scm_stFBInterfaceSpec = {
  2,
  scm_anEventInputNames,
  nullptr,
  nullptr,
  1,
  scm_anEventOutputNames,
  nullptr,
  nullptr,
  0,
  nullptr,
  nullptr,
  0,
  nullptr,
  nullptr,
  0,
  nullptr
};

