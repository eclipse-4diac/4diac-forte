/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#ifndef SRC_MODULES_EMBRICK_TYPES_SLAVE2181_H_
#define SRC_MODULES_EMBRICK_TYPES_SLAVE2181_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_wstring.h>
#include "EBBusAdapter.h"
#include "Slave.h"

class EmbrickSlave2181: public EmbrickSlave {
DECLARE_FIRMWARE_FB(EmbrickSlave2181)

private:
  static const CStringDictionary::TStringId scmDataInputNames[];
  static const CStringDictionary::TStringId scmDataInputTypeIds[];

  CIEC_WSTRING &DigitalInput_1() {
    return *static_cast<CIEC_WSTRING*>(getDI(1));
  }

  CIEC_WSTRING &DigitalInput_2() {
    return *static_cast<CIEC_WSTRING*>(getDI(2));
  }

  CIEC_WSTRING &DigitalInput_3() {
    return *static_cast<CIEC_WSTRING*>(getDI(3));
  }

  CIEC_WSTRING &DigitalInput_4() {
    return *static_cast<CIEC_WSTRING*>(getDI(4));
  }

  CIEC_WSTRING &DigitalInput_5() {
    return *static_cast<CIEC_WSTRING*>(getDI(5));
  }

  CIEC_WSTRING &DigitalInput_6() {
    return *static_cast<CIEC_WSTRING*>(getDI(6));
  }

  CIEC_WSTRING &DigitalInput_7() {
    return *static_cast<CIEC_WSTRING*>(getDI(7));
  }

  CIEC_WSTRING &DigitalInput_8() {
    return *static_cast<CIEC_WSTRING*>(getDI(8));
  }

  CIEC_WSTRING &DigitalOutput_1() {
    return *static_cast<CIEC_WSTRING*>(getDI(9));
  }

  CIEC_WSTRING &DigitalOutput_2() {
    return *static_cast<CIEC_WSTRING*>(getDI(10));
  }

  CIEC_WSTRING &DigitalOutput_3() {
    return *static_cast<CIEC_WSTRING*>(getDI(11));
  }

  CIEC_WSTRING &DigitalOutput_4() {
    return *static_cast<CIEC_WSTRING*>(getDI(12));
  }

  CIEC_WSTRING &DigitalOutput_5() {
    return *static_cast<CIEC_WSTRING*>(getDI(13));
  }

  CIEC_WSTRING &DigitalOutput_6() {
    return *static_cast<CIEC_WSTRING*>(getDI(14));
  }

  CIEC_WSTRING &DigitalOutput_7() {
    return *static_cast<CIEC_WSTRING*>(getDI(15));
  }

  CIEC_WSTRING &DigitalOutput_8() {
    return *static_cast<CIEC_WSTRING*>(getDI(16));
  }

  CIEC_UINT &UpdateInterval() {
    return *static_cast<CIEC_UINT*>(getDI(17));
  }

  static const CStringDictionary::TStringId scmDataOutputNames[];
  static const CStringDictionary::TStringId scmDataOutputTypeIds[];

  static const TForteInt16 scmEIWithIndexes[];
  static const TDataIOID scmEIWith[];
  static const CStringDictionary::TStringId scmEventInputNames[];

  static const TForteInt16 scmEOWithIndexes[];
  static const TDataIOID scmEOWith[];
  static const CStringDictionary::TStringId scmEventOutputNames[];

  static const SAdapterInstanceDef scmAdapterInstances[];

  static const SFBInterfaceSpec scmFBInterfaceSpec;

  static const TForteUInt8 scmSlaveConfigurationIO[];
  static const TForteUInt8 scmSlaveConfigurationIONum;

  virtual void initHandles();

public:
  FUNCTION_BLOCK_CTOR_FOR_IO_MULTI_SLAVE(EmbrickSlave2181, EmbrickSlave, EmbrickSlaveHandler::G_8Di8Do){
};

~EmbrickSlave2181() override = default;

};

#endif //close the ifdef sequence from the beginning of the file

