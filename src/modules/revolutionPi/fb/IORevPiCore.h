/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef _IOREVPICORE_H_
#define _IOREVPICORE_H_

#include <io/configFB/io_master_multi.h>

#include "IORevPiBusAdapter.h"

class FORTE_IORevPiCore: public forte::core::IO::IOConfigFBMultiMaster {
  DECLARE_FIRMWARE_FB(FORTE_IORevPiCore)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  };

  CIEC_UINT &UpdateInterval() {
    return *static_cast<CIEC_UINT*>(getDI(1));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  };

  CIEC_WSTRING &STATUS() {
    return *static_cast<CIEC_WSTRING*>(getDO(1));
  };

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

  FORTE_IORevPiBusAdapter& BusAdapterOut() {
    return (*static_cast<FORTE_IORevPiBusAdapter*>(m_apoAdapters[0]));
  };
  static const int scm_nBusAdapterOutAdpNum = 0;
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(2, 2, 2, 1);

virtual void setInitialValues();

forte::core::IO::IODeviceController* createDeviceController(CDeviceExecution& paDeviceExecution);

  void setConfig();

public:
  FUNCTION_BLOCK_CTOR_WITH_BASE_CLASS(FORTE_IORevPiCore, forte::core::IO::IOConfigFBMultiMaster){
  };

  virtual ~FORTE_IORevPiCore(){};

};

#endif //close the ifdef sequence from the beginning of the file

