/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_MODULES_EMBRICK_TYPES_MASTER_H_
#define SRC_MODULES_EMBRICK_TYPES_MASTER_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_wstring.h>
#include <devlog.h>
#include "BusAdapter.h"

#include <handler/bus.h>

namespace EmBrick {
namespace FunctionBlocks {

class Master: public CFunctionBlock {
DECLARE_FIRMWARE_FB(Master)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  }

  CIEC_WSTRING &PARAMS() {
    return *static_cast<CIEC_WSTRING*>(getDI(1));
  }

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  }

  CIEC_WSTRING &STATUS() {
    return *static_cast<CIEC_WSTRING*>(getDO(1));
  }

  static const TEventID scm_nEventINITID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventINITOID = 0;
  static const TEventID scm_nEventINDID = 1;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SAdapterInstanceDef scm_astAdapterInstances[];

  BusAdapter& BusAdapterOut() {
    return (*static_cast<BusAdapter*>(m_apoAdapters[0]));
  }

  static const int scm_nBusAdapterAdpNum = 0;
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_FB_DATA_ARRAY(2, 2, 2, 1)

  BusHandler *bus;
  void executeEvent(int pa_nEIID);

public:
  FUNCTION_BLOCK_CTOR(Master){
  bus = NULL;
}

virtual ~Master();

};

} /* namespace FunctionsBlocks */
} /* namespace EmBrick */

#endif //close the ifdef sequence from the beginning of the file

