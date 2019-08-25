/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "luacfb.h"
#include "resource.h"

CLuaCFB::CLuaCFB(CStringDictionary::TStringId paInstanceNameId, const CLuaCFBTypeEntry* paTypeEntry, SCFB_FBNData *paFbnData, TForteByte *paConnData,
    TForteByte *paVarsData, CResource *paResource) :
    CCompositeFB(paResource, paTypeEntry->getInterfaceSpec(), paInstanceNameId, paFbnData, paConnData, paVarsData), mTypeEntry(paTypeEntry) {
}

CLuaCFB::~CLuaCFB() {
}
