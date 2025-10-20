/*************************************************************************
 *** Copyright (c) 2012, 2015 Profactor GmbH, fortiss GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: FB_RANDOM
 *** Description: Generate a REAL Randomly
 *** Version:
 ***     1.0: 2012-05-31/Gerhard Ebenhofer - Profactor GmbH - initial API and implementation and/or initial
 *documentation
 ***     1.1: 2015-01-01/Alois Zoitl - fortiss GmbH -
 *************************************************************************/

#pragma once

#include "forte/basicfb.h"
#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include <random>

namespace forte::eclipse4diac::utils {
  class FORTE_FB_RANDOM final : public CBasicFB {
      DECLARE_FIRMWARE_FB(FORTE_FB_RANDOM)

    private:
      static const TEventID scmEventINITID = 0;
      static const TEventID scmEventREQID = 1;
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventCNFID = 1;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_INIT(void);
      void alg_REQ(void);

      static const TForteInt16 scmStateSTART = 0;
      static const TForteInt16 scmStateREQ = 1;
      static const TForteInt16 scmStateState = 2;

      void enterStateSTART(CEventChainExecutionThread *const paECET);
      void enterStateREQ(CEventChainExecutionThread *const paECET);
      void enterStateState(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

      std::random_device mRandomDevice; // Will be used to obtain a seed for the random number engine
      std::mt19937 mRandomGenerator; // Standard mersenne_twister_engine
      std::uniform_real_distribution<float> mDistribution;

    public:
      FORTE_FB_RANDOM(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_UINT var_SEED;

      CIEC_REAL var_VAL;

      CEventConnection conn_INITO;
      CEventConnection conn_CNF;

      CDataConnection *conn_SEED;

      COutDataConnection<CIEC_REAL> conn_VAL;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_INIT(const CIEC_UINT &paSEED, COutputParameter<CIEC_REAL> paVAL) {
        COutputGuard guard_paVAL(paVAL);
        var_SEED = paSEED;
        receiveInputEvent(scmEventINITID, nullptr);
        *paVAL = var_VAL;
      }

      void evt_REQ(const CIEC_UINT &paSEED, COutputParameter<CIEC_REAL> paVAL) {
        COutputGuard guard_paVAL(paVAL);
        var_SEED = paSEED;
        receiveInputEvent(scmEventREQID, nullptr);
        *paVAL = var_VAL;
      }

      void operator()(const CIEC_UINT &paSEED, COutputParameter<CIEC_REAL> paVAL) {
        evt_INIT(paSEED, paVAL);
      }
  };
} // namespace forte::eclipse4diac::utils
