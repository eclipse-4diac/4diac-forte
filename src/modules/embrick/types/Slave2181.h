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

#ifndef SRC_MODULES_EMBRICK_TYPES_SLAVE2181_H_
#define SRC_MODULES_EMBRICK_TYPES_SLAVE2181_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_wstring.h>
#include "BusAdapter.h"
#include "Slave.h"

namespace EmBrick {
namespace FunctionBlocks {

class Slave2181: public Slave {
DECLARE_FIRMWARE_FB(Slave2181)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  }

  CIEC_WSTRING &DigitalInput1() {
    return *static_cast<CIEC_WSTRING*>(getDI(1));
  }

  CIEC_WSTRING &DigitalInput2() {
    return *static_cast<CIEC_WSTRING*>(getDI(2));
  }

  CIEC_WSTRING &DigitalInput3() {
    return *static_cast<CIEC_WSTRING*>(getDI(3));
  }

  CIEC_WSTRING &DigitalInput4() {
    return *static_cast<CIEC_WSTRING*>(getDI(4));
  }

  CIEC_WSTRING &DigitalInput5() {
    return *static_cast<CIEC_WSTRING*>(getDI(5));
  }

  CIEC_WSTRING &DigitalInput6() {
    return *static_cast<CIEC_WSTRING*>(getDI(6));
  }

  CIEC_WSTRING &DigitalInput7() {
    return *static_cast<CIEC_WSTRING*>(getDI(7));
  }

  CIEC_WSTRING &DigitalInput8() {
    return *static_cast<CIEC_WSTRING*>(getDI(8));
  }

  CIEC_WSTRING &DigitalOutput1() {
    return *static_cast<CIEC_WSTRING*>(getDI(9));
  }

  CIEC_WSTRING &DigitalOutput2() {
    return *static_cast<CIEC_WSTRING*>(getDI(10));
  }

  CIEC_WSTRING &DigitalOutput3() {
    return *static_cast<CIEC_WSTRING*>(getDI(11));
  }

  CIEC_WSTRING &DigitalOutput4() {
    return *static_cast<CIEC_WSTRING*>(getDI(12));
  }

  CIEC_WSTRING &DigitalOutput5() {
    return *static_cast<CIEC_WSTRING*>(getDI(13));
  }

  CIEC_WSTRING &DigitalOutput6() {
    return *static_cast<CIEC_WSTRING*>(getDI(14));
  }

  CIEC_WSTRING &DigitalOutput7() {
    return *static_cast<CIEC_WSTRING*>(getDI(15));
  }

  CIEC_WSTRING &DigitalOutput8() {
    return *static_cast<CIEC_WSTRING*>(getDI(16));
  }

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  }

  CIEC_WSTRING &STATUS() {
    return *static_cast<CIEC_WSTRING*>(getDO(1));
  }

  static const TEventID scm_nEventREQID = 0;
  static const TEventID scm_nEventMAPID = 1;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventINDID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SAdapterInstanceDef scm_astAdapterInstances[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_FB_DATA_ARRAY(1, 17, 2, 2)

  virtual void executeEvent(int pa_nEIID);

  virtual bool init(int index);

public:
  FUNCTION_BLOCK_CTOR_WITH_BASE_CLASS(Slave2181, Slave){
  type = G_8Di8Do;
};

virtual ~Slave2181() {};

};

} /* namespace FunctionsBlocks */
} /* namespace EmBrick */

#endif //close the ifdef sequence from the beginning of the file

