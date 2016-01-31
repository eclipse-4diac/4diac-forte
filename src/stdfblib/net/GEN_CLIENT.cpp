/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Rene Smodic, Gerhard Ebenhofer, Ingo Hegny,
 *   Martin Melik Merkumians
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "GEN_CLIENT.h"

DEFINE_GENERIC_FIRMWARE_FB(GEN_CLIENT, g_nStringIdGEN_CLIENT)

GEN_CLIENT::GEN_CLIENT(const CStringDictionary::TStringId pa_nInstanceNameId,
    CResource *pa_poSrcRes):
    forte::com_infra::CCommFB(pa_nInstanceNameId, pa_poSrcRes, forte::com_infra::e_Client){
    //CCommFB(pa_nInstanceNameId, pa_poSrcRes, true, scm_nSendBufferSize, scm_nRecvBufferSize, CCommFB::e_Client){
}
