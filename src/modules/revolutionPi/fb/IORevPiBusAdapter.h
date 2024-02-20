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

#ifndef _IOREVPIBUSADAPTER_H_
#define _IOREVPIBUSADAPTER_H_

#include <io/configFB/io_adapter_multi.h>
#include <typelib.h>
#include <forte_bool.h>
#include <forte_uint.h>

class FORTE_IORevPiBusAdapter: public forte::core::IO::IOConfigFBMultiAdapter {
  DECLARE_ADAPTER_TYPE(FORTE_IORevPiBusAdapter)

private:
 private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
 public:
  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>((isSocket()) ? getDI(0) : getDO(0));
  };

 private:
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
 public:
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>((isSocket()) ? getDO(0) : getDI(0));
  };

  CIEC_UINT &MasterId() {
    return *static_cast<CIEC_UINT*>((isSocket()) ? getDO(1) : getDI(1));
  };

  CIEC_UINT &Index() {
    return *static_cast<CIEC_UINT*>((isSocket()) ? getDO(2) : getDI(2));
  };

 public:
  static const TEventID scm_nEventINITOID = 0;
  int INITO() {
    return m_nParentAdapterListEventID + scm_nEventINITOID;
  }
 private:
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

 public:
  static const TEventID scm_nEventINITID = 0;
  int INIT() {
    return m_nParentAdapterListEventID + scm_nEventINITID;
  }
 private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpecSocket;

  static const SFBInterfaceSpec scm_stFBInterfaceSpecPlug;

   FORTE_ADAPTER_DATA_ARRAY(1, 1, 1, 3, 0);

public:
  ADAPTER_CTOR_FOR_IO_MULTI (FORTE_IORevPiBusAdapter) {
  };
  static const TForteUInt8 scm_slaveConfigurationIO[];
  static const TForteUInt8 scm_slaveConfigurationIO_num;

  virtual ~FORTE_IORevPiBusAdapter(){};

};

#endif //close the ifdef sequence from the beginning of the file

