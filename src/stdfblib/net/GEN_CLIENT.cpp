/*******************************************************************************
 * Copyright (c) 2006 - 2011 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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
