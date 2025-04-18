/*************************************************************************
 * Copyright (c) 2015, 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Gerd Kainz - initial API and implementation and/or initial documentation
 *   Jose Cabral - Modification to double
 *************************************************************************/

#pragma once

#include "funcbloc.h"
#include "forte_bool.h"
#include "forte_dword.h"
#include "forte_string.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

#include "processinterface.h"

/*! /brief generic class for QW function blocks providing access to one word physical output
 *
 * In order that this FB can be used implementations need to provide a class CProcessInterface
 * this class needs to inherit from CCProcessInterfaceBase and provide the following functions
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
 *   - bool writeDWord()
 *     Write the value of function block's IN() data input to the physical input
 *       - @return true on success, false on error
 *
 * TODO a higher flexibility and easier use could be achieve if the base class would be a template parameter. However
 *   currently it is very hard to templatize a function block class.
 */
class FORTE_QD final : public CProcessInterface {
    DECLARE_FIRMWARE_FB(FORTE_QD)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventREQID = 1;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const CStringDictionary::TStringId scmEventInputTypeIds[];
    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventCNFID = 1;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];
    static const CStringDictionary::TStringId scmEventOutputTypeIds[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_QD(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_STRING var_PARAMS;
    CIEC_DWORD var_OUT;

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

    void evt_INIT(const CIEC_BOOL &paQI,
                  const CIEC_STRING &paPARAMS,
                  const CIEC_DWORD &paOUT,
                  CIEC_BOOL &paQO,
                  CIEC_STRING &paSTATUS) {
      var_QI = paQI;
      var_PARAMS = paPARAMS;
      var_OUT = paOUT;
      receiveInputEvent(scmEventINITID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
    }

    void evt_REQ(const CIEC_BOOL &paQI,
                 const CIEC_STRING &paPARAMS,
                 const CIEC_DWORD &paOUT,
                 CIEC_BOOL &paQO,
                 CIEC_STRING &paSTATUS) {
      var_QI = paQI;
      var_PARAMS = paPARAMS;
      var_OUT = paOUT;
      receiveInputEvent(scmEventREQID, nullptr);
      paQO = var_QO;
      paSTATUS = var_STATUS;
    }

    void operator()(const CIEC_BOOL &paQI,
                    const CIEC_STRING &paPARAMS,
                    const CIEC_DWORD &paOUT,
                    CIEC_BOOL &paQO,
                    CIEC_STRING &paSTATUS) {
      evt_INIT(paQI, paPARAMS, paOUT, paQO, paSTATUS);
    }
};
