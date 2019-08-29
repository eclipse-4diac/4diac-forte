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
#include "E_SPLIT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SPLIT_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(E_SPLIT, g_nStringIdE_SPLIT)

const CStringDictionary::TStringId E_SPLIT::scm_anEventInputNames[] = {g_nStringIdEI};

const CStringDictionary::TStringId E_SPLIT::scm_anEventOutputNames[] = {g_nStringIdEO1, g_nStringIdEO2};

const SFBInterfaceSpec E_SPLIT::scm_stFBInterfaceSpec = {
  1,
  scm_anEventInputNames,
  0,
  0,
  2,
  scm_anEventOutputNames,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};
