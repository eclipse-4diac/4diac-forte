/*******************************************************************************
 * Copyright (c) 2014 - 2015 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Waldemar Eisenmenger - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_string.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

#include "core/fmi/processinterface.h"

/*! /brief generic namespace forte::eclipse4diac::io {
class for QX function blocks providing access to one boolean physical output
 *
 * In order that this FB can be used implementations need to provide a namespace forte::eclipse4diac::io {
class CProcessInterface
 * this namespace forte::eclipse4diac::io {
class needs to inherit from CProcessInterfaceBase and provide the following functions
 *
 *   - bool initialise(bool paInput, CEventChainExecutionThread *const paECET)
 *     Initialize the physical output identified with the value of the PARAMS FB input
 *       - @param paInput: if true it should be an input, if false it should be an output
 *       - @return true on success, false on error
 *
 *   - bool deinitialise()
 *     Deinitalize the physical port
 *       - @return true on success, false on error
 *
 *   - bool writePin()
 *     Write the value of function block's IN() data input to the physical input
 *       - @return true on success, false on error
 *
 * TODO a higher flexibility and easier use could be achieve if the base namespace forte::eclipse4diac::io {
class would be a template parameter. However
 *   currently it is very hard to templatize a function block namespace forte::eclipse4diac::io {
class.
 */
namespace forte::eclipse4diac::io {
  class FORTE_QX final : public CProcessInterface {
      DECLARE_FIRMWARE_FB(FORTE_QX)

    private:
      static const TEventID scmEventINITID = 0;
      static const TEventID scmEventREQID = 1;
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventCNFID = 1;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_QX(const forte::StringId paInstanceNameId, forte::CFBContainer &paContainer);

      CIEC_BOOL var_QI;
      CIEC_STRING var_PARAMS;
      CIEC_BOOL var_OUT;
      CIEC_BOOL var_QO;
      CIEC_STRING var_STATUS;
      CEventConnection conn_INITO;
      CEventConnection conn_CNF;
      CDataConnection *conn_QI;
      CDataConnection *conn_PARAMS;
      CDataConnection *conn_OUT;
      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_STRING> conn_STATUS;
      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;
      void evt_INIT(const CIEC_BOOL &pa_QI,
                    const CIEC_STRING &pa_PARAMS,
                    const CIEC_BOOL &pa_OUT,
                    CAnyBitOutputParameter<CIEC_BOOL> pa_QO,
                    COutputParameter<CIEC_STRING> pa_STATUS) {
        COutputGuard guard_pa_QO(pa_QO);
        COutputGuard guard_pa_STATUS(pa_STATUS);
        var_QI = pa_QI;
        var_PARAMS = pa_PARAMS;
        var_OUT = pa_OUT;
        receiveInputEvent(scmEventINITID, nullptr);
        *pa_QO = var_QO;
        *pa_STATUS = var_STATUS;
      }
      void evt_REQ(const CIEC_BOOL &pa_QI,
                   const CIEC_STRING &pa_PARAMS,
                   const CIEC_BOOL &pa_OUT,
                   CAnyBitOutputParameter<CIEC_BOOL> pa_QO,
                   COutputParameter<CIEC_STRING> pa_STATUS) {
        COutputGuard guard_pa_QO(pa_QO);
        COutputGuard guard_pa_STATUS(pa_STATUS);
        var_QI = pa_QI;
        var_PARAMS = pa_PARAMS;
        var_OUT = pa_OUT;
        receiveInputEvent(scmEventREQID, nullptr);
        *pa_QO = var_QO;
        *pa_STATUS = var_STATUS;
      }
      void operator()(const CIEC_BOOL &pa_QI,
                      const CIEC_STRING &pa_PARAMS,
                      const CIEC_BOOL &pa_OUT,
                      CAnyBitOutputParameter<CIEC_BOOL> pa_QO,
                      COutputParameter<CIEC_STRING> pa_STATUS) {
        evt_INIT(pa_QI, pa_PARAMS, pa_OUT, pa_QO, pa_STATUS);
      }
  };
} // namespace forte::eclipse4diac::io
