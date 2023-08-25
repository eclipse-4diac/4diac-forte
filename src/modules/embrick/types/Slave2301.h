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

#ifndef SRC_MODULES_EMBRICK_TYPES_SLAVE2301_H_
#define SRC_MODULES_EMBRICK_TYPES_SLAVE2301_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_wstring.h>
#include "BusAdapter.h"
#include "Slave.h"

class EmbrickSlave2301: public EmbrickSlave {
DECLARE_FIRMWARE_FB(EmbrickSlave2301)

private:
  static const CStringDictionary::TStringId scmDataInputNames[];
  static const CStringDictionary::TStringId scmDataInputTypeIds[];

  CIEC_WSTRING &Relay_1() {
    return *static_cast<CIEC_WSTRING*>(getDI(1));
  }

  CIEC_WSTRING &Relay_2() {
    return *static_cast<CIEC_WSTRING*>(getDI(2));
  }

  CIEC_WSTRING &Relay_3() {
    return *static_cast<CIEC_WSTRING*>(getDI(3));
  }

  CIEC_WSTRING &Relay_4() {
    return *static_cast<CIEC_WSTRING*>(getDI(4));
  }

  CIEC_WSTRING &Relay_5() {
    return *static_cast<CIEC_WSTRING*>(getDI(5));
  }

  CIEC_WSTRING &Relay_6() {
    return *static_cast<CIEC_WSTRING*>(getDI(6));
  }

  CIEC_UINT &UpdateInterval() {
    return *static_cast<CIEC_UINT*>(getDI(7));
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
  FUNCTION_BLOCK_CTOR_FOR_IO_MULTI_SLAVE(EmbrickSlave2301, EmbrickSlave, EmbrickSlaveHandler::G_2RelNo4RelCo){
};

~EmbrickSlave2301() override = default;

};

#endif //close the ifdef sequence from the beginning of the file

