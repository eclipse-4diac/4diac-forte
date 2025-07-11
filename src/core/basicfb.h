/*******************************************************************************
 * Copyright (c) 2005, 2025 Profactor GmbH, ACIN, fortiss GmbH,
 *                          Martin Erich Jobst, Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Gunnar Grabmair, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *   Martin Jobst - account for data type size in FB initialization
 *   Alois Zoitl  - exracted internal variable handling to new CBaseFB
 *******************************************************************************/

#pragma once

#include "core/basefb.h"
#include "core/datatypes/forte_state.h"

/*!\ingroup CORE
 *
 * \brief Class for handling firmware basic function blocks.
 */
class CBasicFB : public CBaseFB {
  public:
    ~CBasicFB() override = default;

    CIEC_ANY *getVar(CStringDictionary::TStringId *paNameList, unsigned int paNameListSize) override;

  protected:
    CBasicFB(forte::core::CFBContainer &paContainer,
             const SFBInterfaceSpec &paInterfaceSpec,
             CStringDictionary::TStringId paInstanceNameId,
             std::span<const CStringDictionary::TStringId> paVarInternalNames);

    void setInitialValues() override = 0;

    CIEC_STATE mECCState; //! the current state of the ecc. start value is 0 = initial state id

#ifdef FORTE_FMU
    friend class fmuInstance;
#endif // FORTE_FMU
};
