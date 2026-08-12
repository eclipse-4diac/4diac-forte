/*************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.2.100.202608212006!
 ***
 *** Name: RampLimitFS
 *** Description: Setpoint Ramp: Step up and down Values with Fast and Slow mode
 *** Version:
 ***     3.1: 2026-08-12/Franz Höpfinger - HR Agrartechnik GmbH - add init and correct WITH, add upper and lower limit
 ***                                       reached indicators
 ***     3.0: 2025-04-14/Patrick Aigner  - changed package
 ***     1.1: 2024-10-02/Franz Höpfinger - HR Agrartechnik GmbH - Rename to RampLimitFS
 ***     1.0: 2024-09-20/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#pragma once

#include "forte/simplefb.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_dint.h"
#include "forte/forte_st_util.h"

namespace forte::eclipse4diac::signalprocessing {
  class FORTE_RampLimitFS final : public CSimpleFB {
      DECLARE_FIRMWARE_FB(FORTE_RampLimitFS)

    private:
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventCNFID = 1;
      static const TEventID scmEventINITID = 0;
      static const TEventID scmEventZEROID = 1;
      static const TEventID scmEventUP_SLOWID = 2;
      static const TEventID scmEventUP_FASTID = 3;
      static const TEventID scmEventDOWN_SLOWID = 4;
      static const TEventID scmEventDOWN_FASTID = 5;
      static const TEventID scmEventFULLID = 6;
      static const TEventID scmEventLOADID = 7;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_INIT(void);
      void alg_ZERO(void);
      void alg_UP_SLOW(void);
      void alg_UP_FAST(void);
      void alg_DOWN_SLOW(void);
      void alg_DOWN_FAST(void);
      void alg_FULL(void);
      void alg_LOAD(void);

      void enterStateINIT(CEventChainExecutionThread *const paECET);
      void enterStateZERO(CEventChainExecutionThread *const paECET);
      void enterStateUP_SLOW(CEventChainExecutionThread *const paECET);
      void enterStateUP_FAST(CEventChainExecutionThread *const paECET);
      void enterStateDOWN_SLOW(CEventChainExecutionThread *const paECET);
      void enterStateDOWN_FAST(CEventChainExecutionThread *const paECET);
      void enterStateFULL(CEventChainExecutionThread *const paECET);
      void enterStateLOAD(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_RampLimitFS(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_DINT var_PV;
      CIEC_DINT var_VAL_ZERO;
      CIEC_DINT var_SLOW;
      CIEC_DINT var_FAST;
      CIEC_DINT var_VAL_FULL;

      CIEC_DINT var_OUT;
      CIEC_BOOL var_qAtZero;
      CIEC_BOOL var_qAtFull;

      CEventConnection conn_INITO;
      CEventConnection conn_CNF;

      CDataConnection *conn_PV;
      CDataConnection *conn_VAL_ZERO;
      CDataConnection *conn_SLOW;
      CDataConnection *conn_FAST;
      CDataConnection *conn_VAL_FULL;

      COutDataConnection<CIEC_DINT> conn_OUT;
      COutDataConnection<CIEC_BOOL> conn_qAtZero;
      COutDataConnection<CIEC_BOOL> conn_qAtFull;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_INIT(const CIEC_DINT &paPV,
                    const CIEC_DINT &paVAL_ZERO,
                    const CIEC_DINT &paSLOW,
                    const CIEC_DINT &paFAST,
                    const CIEC_DINT &paVAL_FULL,
                    COutputParameter<CIEC_DINT> paOUT,
                    CAnyBitOutputParameter<CIEC_BOOL> paqAtZero,
                    CAnyBitOutputParameter<CIEC_BOOL> paqAtFull) {
        COutputGuard guard_OUT(paOUT);
        COutputGuard guard_qAtZero(paqAtZero);
        COutputGuard guard_qAtFull(paqAtFull);
        var_PV = paPV;
        var_VAL_ZERO = paVAL_ZERO;
        var_SLOW = paSLOW;
        var_FAST = paFAST;
        var_VAL_FULL = paVAL_FULL;
        executeEvent(scmEventINITID, nullptr);
        *paOUT = var_OUT;
        *paqAtZero = var_qAtZero;
        *paqAtFull = var_qAtFull;
      }

      void evt_ZERO(const CIEC_DINT &paPV,
                    const CIEC_DINT &paVAL_ZERO,
                    const CIEC_DINT &paSLOW,
                    const CIEC_DINT &paFAST,
                    const CIEC_DINT &paVAL_FULL,
                    COutputParameter<CIEC_DINT> paOUT,
                    CAnyBitOutputParameter<CIEC_BOOL> paqAtZero,
                    CAnyBitOutputParameter<CIEC_BOOL> paqAtFull) {
        COutputGuard guard_OUT(paOUT);
        COutputGuard guard_qAtZero(paqAtZero);
        COutputGuard guard_qAtFull(paqAtFull);
        var_PV = paPV;
        var_VAL_ZERO = paVAL_ZERO;
        var_SLOW = paSLOW;
        var_FAST = paFAST;
        var_VAL_FULL = paVAL_FULL;
        executeEvent(scmEventZEROID, nullptr);
        *paOUT = var_OUT;
        *paqAtZero = var_qAtZero;
        *paqAtFull = var_qAtFull;
      }

      void evt_UP_SLOW(const CIEC_DINT &paPV,
                       const CIEC_DINT &paVAL_ZERO,
                       const CIEC_DINT &paSLOW,
                       const CIEC_DINT &paFAST,
                       const CIEC_DINT &paVAL_FULL,
                       COutputParameter<CIEC_DINT> paOUT,
                       CAnyBitOutputParameter<CIEC_BOOL> paqAtZero,
                       CAnyBitOutputParameter<CIEC_BOOL> paqAtFull) {
        COutputGuard guard_OUT(paOUT);
        COutputGuard guard_qAtZero(paqAtZero);
        COutputGuard guard_qAtFull(paqAtFull);
        var_PV = paPV;
        var_VAL_ZERO = paVAL_ZERO;
        var_SLOW = paSLOW;
        var_FAST = paFAST;
        var_VAL_FULL = paVAL_FULL;
        executeEvent(scmEventUP_SLOWID, nullptr);
        *paOUT = var_OUT;
        *paqAtZero = var_qAtZero;
        *paqAtFull = var_qAtFull;
      }

      void evt_UP_FAST(const CIEC_DINT &paPV,
                       const CIEC_DINT &paVAL_ZERO,
                       const CIEC_DINT &paSLOW,
                       const CIEC_DINT &paFAST,
                       const CIEC_DINT &paVAL_FULL,
                       COutputParameter<CIEC_DINT> paOUT,
                       CAnyBitOutputParameter<CIEC_BOOL> paqAtZero,
                       CAnyBitOutputParameter<CIEC_BOOL> paqAtFull) {
        COutputGuard guard_OUT(paOUT);
        COutputGuard guard_qAtZero(paqAtZero);
        COutputGuard guard_qAtFull(paqAtFull);
        var_PV = paPV;
        var_VAL_ZERO = paVAL_ZERO;
        var_SLOW = paSLOW;
        var_FAST = paFAST;
        var_VAL_FULL = paVAL_FULL;
        executeEvent(scmEventUP_FASTID, nullptr);
        *paOUT = var_OUT;
        *paqAtZero = var_qAtZero;
        *paqAtFull = var_qAtFull;
      }

      void evt_DOWN_SLOW(const CIEC_DINT &paPV,
                         const CIEC_DINT &paVAL_ZERO,
                         const CIEC_DINT &paSLOW,
                         const CIEC_DINT &paFAST,
                         const CIEC_DINT &paVAL_FULL,
                         COutputParameter<CIEC_DINT> paOUT,
                         CAnyBitOutputParameter<CIEC_BOOL> paqAtZero,
                         CAnyBitOutputParameter<CIEC_BOOL> paqAtFull) {
        COutputGuard guard_OUT(paOUT);
        COutputGuard guard_qAtZero(paqAtZero);
        COutputGuard guard_qAtFull(paqAtFull);
        var_PV = paPV;
        var_VAL_ZERO = paVAL_ZERO;
        var_SLOW = paSLOW;
        var_FAST = paFAST;
        var_VAL_FULL = paVAL_FULL;
        executeEvent(scmEventDOWN_SLOWID, nullptr);
        *paOUT = var_OUT;
        *paqAtZero = var_qAtZero;
        *paqAtFull = var_qAtFull;
      }

      void evt_DOWN_FAST(const CIEC_DINT &paPV,
                         const CIEC_DINT &paVAL_ZERO,
                         const CIEC_DINT &paSLOW,
                         const CIEC_DINT &paFAST,
                         const CIEC_DINT &paVAL_FULL,
                         COutputParameter<CIEC_DINT> paOUT,
                         CAnyBitOutputParameter<CIEC_BOOL> paqAtZero,
                         CAnyBitOutputParameter<CIEC_BOOL> paqAtFull) {
        COutputGuard guard_OUT(paOUT);
        COutputGuard guard_qAtZero(paqAtZero);
        COutputGuard guard_qAtFull(paqAtFull);
        var_PV = paPV;
        var_VAL_ZERO = paVAL_ZERO;
        var_SLOW = paSLOW;
        var_FAST = paFAST;
        var_VAL_FULL = paVAL_FULL;
        executeEvent(scmEventDOWN_FASTID, nullptr);
        *paOUT = var_OUT;
        *paqAtZero = var_qAtZero;
        *paqAtFull = var_qAtFull;
      }

      void evt_FULL(const CIEC_DINT &paPV,
                    const CIEC_DINT &paVAL_ZERO,
                    const CIEC_DINT &paSLOW,
                    const CIEC_DINT &paFAST,
                    const CIEC_DINT &paVAL_FULL,
                    COutputParameter<CIEC_DINT> paOUT,
                    CAnyBitOutputParameter<CIEC_BOOL> paqAtZero,
                    CAnyBitOutputParameter<CIEC_BOOL> paqAtFull) {
        COutputGuard guard_OUT(paOUT);
        COutputGuard guard_qAtZero(paqAtZero);
        COutputGuard guard_qAtFull(paqAtFull);
        var_PV = paPV;
        var_VAL_ZERO = paVAL_ZERO;
        var_SLOW = paSLOW;
        var_FAST = paFAST;
        var_VAL_FULL = paVAL_FULL;
        executeEvent(scmEventFULLID, nullptr);
        *paOUT = var_OUT;
        *paqAtZero = var_qAtZero;
        *paqAtFull = var_qAtFull;
      }

      void evt_LOAD(const CIEC_DINT &paPV,
                    const CIEC_DINT &paVAL_ZERO,
                    const CIEC_DINT &paSLOW,
                    const CIEC_DINT &paFAST,
                    const CIEC_DINT &paVAL_FULL,
                    COutputParameter<CIEC_DINT> paOUT,
                    CAnyBitOutputParameter<CIEC_BOOL> paqAtZero,
                    CAnyBitOutputParameter<CIEC_BOOL> paqAtFull) {
        COutputGuard guard_OUT(paOUT);
        COutputGuard guard_qAtZero(paqAtZero);
        COutputGuard guard_qAtFull(paqAtFull);
        var_PV = paPV;
        var_VAL_ZERO = paVAL_ZERO;
        var_SLOW = paSLOW;
        var_FAST = paFAST;
        var_VAL_FULL = paVAL_FULL;
        executeEvent(scmEventLOADID, nullptr);
        *paOUT = var_OUT;
        *paqAtZero = var_qAtZero;
        *paqAtFull = var_qAtFull;
      }
  };
} // namespace forte::eclipse4diac::signalprocessing
