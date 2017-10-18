/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "luacfb.h"
#include "resource.h"

CLuaCFB::CLuaCFB(CStringDictionary::TStringId instanceNameId, const CLuaCFBTypeEntry* typeEntry, SCFB_FBNData *fbnData, TForteByte *connData, TForteByte *varsData, CResource *resource)
    : CCompositeFB(resource, typeEntry->getInterfaceSpec(), instanceNameId, fbnData, connData, varsData), typeEntry(typeEntry) {
}

CLuaCFB::~CLuaCFB() {
}
