/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH
 *                      2018 Johannes Kepler University
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#ifndef _GEN_ADD_H_
#define _GEN_ADD_H_

#include <genfb.h>
#include <mixedStorage.h>
#include <forte_any_magnitude_variant.h>

class GEN_ADD : public CGenFunctionBlock<CFunctionBlock> {
  DECLARE_GENERIC_FIRMWARE_FB(GEN_ADD)

  private:
    forte::core::util::CMixedStorage m_oIfSpecStorage;

    CIEC_ANY_MAGNITUDE_VARIANT& var_IN(size_t paIndex) {
      return *static_cast<CIEC_ANY_MAGNITUDE_VARIANT*>(getDI(paIndex));
    }

    CIEC_ANY_MAGNITUDE_VARIANT& var_OUT() {
      return *static_cast<CIEC_ANY_MAGNITUDE_VARIANT*>(getDO(0));
    }

    static const TEventID scm_nEventREQID = 0;
    static const TForteInt16 scm_anEIWithIndexes[];
    static const CStringDictionary::TStringId scm_anEventInputNames[];

    static const TEventID scm_nEventCNFID = 0;
    static const TForteInt16 scm_anEOWithIndexes[];
    static const TDataIOID scm_anEOWith[];
    static const CStringDictionary::TStringId scm_anEventOutputNames[];

    //self-defined members
    unsigned int m_nDInputs;

    void executeEvent(TEventID paEIID) override;
    bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

    GEN_ADD(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);
    ~GEN_ADD() override;
};

#endif // _GEN_ADD_H_

