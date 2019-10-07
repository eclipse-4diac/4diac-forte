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
#include "BusAdapter.h"

#include <handler/bus.h>
#include "../../../core/io/configFB/io_master_multi.h"

class EmbrickMaster: public forte::core::io::IOConfigFBMultiMaster {
DECLARE_FIRMWARE_FB(EmbrickMaster)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];

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

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];

  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SAdapterInstanceDef scm_astAdapterInstances[];

  EmbrickBusAdapter& BusAdapterOut() {
    return (*static_cast<EmbrickBusAdapter*>(m_apoAdapters[0]));
  }

  static const int scm_nBusAdapterAdpNum = 0;
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_FB_DATA_ARRAY(2, 6, 2, 1)

  virtual void setInitialValues();

protected:
  forte::core::io::IODeviceController* createDeviceController(CDeviceExecution& paDeviceExecution);

  void setConfig();

  virtual void onStartup();

public:
  FUNCTION_BLOCK_CTOR_WITH_BASE_CLASS(EmbrickMaster, forte::core::io::IOConfigFBMultiMaster){
  }
};


#endif //close the ifdef sequence from the beginning of the file

