/*************************************************************************
 * Copyright (c) 2015, 2025 fortiss GmbH, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Gerd Kainz - initial API and implementation and/or initial documentation
 *   Alois Zoitl - copied to core/io and adjusted to core/io process
 *                 interface needs
 *******************************************************************************/

#pragma once

#include "inputfb.h"
#include "forte_word.h"

/*! /brief generic class for IW function blocks providing access to one word physical input
 *
 */
class FORTE_IW final : public forte::core::io::CInputFB<CIEC_WORD> {
  DECLARE_FIRMWARE_FB(FORTE_IW)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const CStringDictionary::TStringId scmEventInputTypeIds[];
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];
    static const CStringDictionary::TStringId scmEventOutputTypeIds[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

  public:
    FORTE_IW(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

};

