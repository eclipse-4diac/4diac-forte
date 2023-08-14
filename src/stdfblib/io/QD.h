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
#include "forte_string.h"
#include "forte_dword.h"
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
 *   - bool initialise(bool paInput)
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
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  static const TEventID scm_nEventINITID = 0;
  static const TEventID scm_nEventREQID = 1;
  static const TDataIOID scm_anEIWith[];
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];
  static const TEventID scm_nEventINITOID = 0;
  static const TEventID scm_nEventCNFID = 1;
  static const TDataIOID scm_anEOWith[]; 
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

  void readInputData(TEventID paEIID) override;
  void writeOutputData(TEventID paEIID) override;
  void setInitialValues() override;

public:
  FORTE_QD(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  CIEC_BOOL var_QI;
  CIEC_STRING var_PARAMS;
  CIEC_DWORD var_OUT;
  CIEC_BOOL var_QO;
  CIEC_STRING var_STATUS;
  CIEC_BOOL var_conn_QO;
  CIEC_STRING var_conn_STATUS;
  CEventConnection conn_INITO;
  CEventConnection conn_CNF;
  CDataConnection *conn_QI;
  CDataConnection *conn_PARAMS;
  CDataConnection *conn_OUT;
  CDataConnection conn_QO;
  CDataConnection conn_STATUS;
  CIEC_ANY *getDI(size_t) override;
  CIEC_ANY *getDO(size_t) override;
  CIEC_ANY *getDIO(size_t) override;
  CEventConnection *getEOConUnchecked(TPortId) override;
  CDataConnection **getDIConUnchecked(TPortId) override;
  CDataConnection *getDOConUnchecked(TPortId) override;
  CInOutDataConnection **getDIOInConUnchecked(TPortId) override;
  CInOutDataConnection *getDIOOutConUnchecked(TPortId) override;
  void evt_INIT(const CIEC_BOOL &pa_QI, const CIEC_STRING &pa_PARAMS, const CIEC_DWORD &pa_OUT, CIEC_BOOL &pa_QO, CIEC_STRING &pa_STATUS) {
    var_QI = pa_QI;
    var_PARAMS = pa_PARAMS;
    var_OUT = pa_OUT;
    receiveInputEvent(scm_nEventINITID, nullptr);
    pa_QO = var_QO;
    pa_STATUS = var_STATUS;
  }
  void evt_REQ(const CIEC_BOOL &pa_QI, const CIEC_STRING &pa_PARAMS, const CIEC_DWORD &pa_OUT, CIEC_BOOL &pa_QO, CIEC_STRING &pa_STATUS) {
    var_QI = pa_QI;
    var_PARAMS = pa_PARAMS;
    var_OUT = pa_OUT;
    receiveInputEvent(scm_nEventREQID, nullptr);
    pa_QO = var_QO;
    pa_STATUS = var_STATUS;
  }
  void operator()(const CIEC_BOOL &pa_QI, const CIEC_STRING &pa_PARAMS, const CIEC_DWORD &pa_OUT, CIEC_BOOL &pa_QO, CIEC_STRING &pa_STATUS) {
    evt_INIT(pa_QI, pa_PARAMS, pa_OUT, pa_QO, pa_STATUS);
  }
};



