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

#ifndef SRC_MODULES_EMBRICK_TYPES_MASTER_H_
#define SRC_MODULES_EMBRICK_TYPES_MASTER_H_

#include <forte_bool.h>
#include <forte_wstring.h>
#include <devlog.h>
#include <handler/bus.h>
#include "../../../core/io/configFB/io_master_multi.h"
#include "EBBusAdapter.h"

class FORTE_EBMaster: public forte::core::io::IOConfigFBMultiMaster {
DECLARE_FIRMWARE_FB(FORTE_EBMaster)

private:
  static const CStringDictionary::TStringId scmDataInputNames[];
  static const CStringDictionary::TStringId scmDataInputTypeIds[];

  CIEC_UINT &BusInterface() {
    return *static_cast<CIEC_UINT*>(getDI(1));
  }

  CIEC_UINT &BusSelectPin() {
    return *static_cast<CIEC_UINT*>(getDI(2));
  }

  CIEC_UDINT &BusInitSpeed() {
    return *static_cast<CIEC_UDINT*>(getDI(3));
  }

  CIEC_UDINT &BusLoopSpeed() {
    return *static_cast<CIEC_UDINT*>(getDI(4));
  }

  CIEC_UINT &SlaveUpdateInterval() {
    return *static_cast<CIEC_UINT*>(getDI(5));
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

  FORTE_EBBusAdapter& BusAdapterOut() {
    return (*static_cast<EBBusAdapter*>(mAdapters[0]));
  }

  static const int scmBusAdapterAdpNum = 0;
  static const SFBInterfaceSpec scmFBInterfaceSpec;

  virtual void setInitialValues();

protected:
  forte::core::io::IODeviceController* createDeviceController(CDeviceExecution& paDeviceExecution);

  void setConfig();

  virtual void onStartup();

public:
  FUNCTION_BLOCK_CTOR_WITH_BASE_CLASS(FORTE_EBMaster, forte::core::io::IOConfigFBMultiMaster){
  }
};


#endif //close the ifdef sequence from the beginning of the file

