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

#ifndef _IOREVPIDIO_H_
#define _IOREVPIDIO_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_wstring.h>
#include "IORevPiBusAdapter.h"
#include <io/configFB/io_slave_multi.h>

class FORTE_IORevPiDIO: public forte::core::IO::IOConfigFBMultiSlave {
DECLARE_FIRMWARE_FB(FORTE_IORevPiDIO)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  }

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

  CIEC_WSTRING &DigitalInput_9() {
    return *static_cast<CIEC_WSTRING*>(getDI(9));
  }

  CIEC_WSTRING &DigitalInput_10() {
    return *static_cast<CIEC_WSTRING*>(getDI(10));
  }

  CIEC_WSTRING &DigitalInput_11() {
    return *static_cast<CIEC_WSTRING*>(getDI(11));
  }

  CIEC_WSTRING &DigitalInput_12() {
    return *static_cast<CIEC_WSTRING*>(getDI(12));
  }

  CIEC_WSTRING &DigitalInput_13() {
    return *static_cast<CIEC_WSTRING*>(getDI(13));
  }

  CIEC_WSTRING &DigitalInput_14() {
    return *static_cast<CIEC_WSTRING*>(getDI(14));
  }

  CIEC_WSTRING &DigitalOutput_1() {
    return *static_cast<CIEC_WSTRING*>(getDI(15));
  }

  CIEC_WSTRING &DigitalOutput_2() {
    return *static_cast<CIEC_WSTRING*>(getDI(16));
  }

  CIEC_WSTRING &DigitalOutput_3() {
    return *static_cast<CIEC_WSTRING*>(getDI(17));
  }

  CIEC_WSTRING &DigitalOutput_4() {
    return *static_cast<CIEC_WSTRING*>(getDI(18));
  }

  CIEC_WSTRING &DigitalOutput_5() {
    return *static_cast<CIEC_WSTRING*>(getDI(19));
  }

  CIEC_WSTRING &DigitalOutput_6() {
    return *static_cast<CIEC_WSTRING*>(getDI(20));
  }

  CIEC_WSTRING &DigitalOutput_7() {
    return *static_cast<CIEC_WSTRING*>(getDI(21));
  }

  CIEC_WSTRING &DigitalOutput_8() {
    return *static_cast<CIEC_WSTRING*>(getDI(22));
  }

  CIEC_WSTRING &DigitalOutput_9() {
    return *static_cast<CIEC_WSTRING*>(getDI(23));
  }

  CIEC_WSTRING &DigitalOutput_10() {
    return *static_cast<CIEC_WSTRING*>(getDI(24));
  }

  CIEC_WSTRING &DigitalOutput_11() {
    return *static_cast<CIEC_WSTRING*>(getDI(25));
  }

  CIEC_WSTRING &DigitalOutput_12() {
    return *static_cast<CIEC_WSTRING*>(getDI(26));
  }

  CIEC_WSTRING &DigitalOutput_13() {
    return *static_cast<CIEC_WSTRING*>(getDI(27));
  }

  CIEC_WSTRING &DigitalOutput_14() {
    return *static_cast<CIEC_WSTRING*>(getDI(28));
  }

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  }

  CIEC_WSTRING &STATUS() {
    return *static_cast<CIEC_WSTRING*>(getDO(1));
  }

  static const TEventID scm_nEventMAPID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventMAPOID = 0;
  static const TEventID scm_nEventINDID = 1;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SAdapterInstanceDef scm_astAdapterInstances[];

  FORTE_IORevPiBusAdapter& BusAdapterOut() {
    return (*static_cast<FORTE_IORevPiBusAdapter*>(m_apoAdapters[0]));
  }

  static const int scm_nBusAdapterOutAdpNum = 0;
  FORTE_IORevPiBusAdapter& BusAdapterIn() {
    return (*static_cast<FORTE_IORevPiBusAdapter*>(m_apoAdapters[1]));
  }

  static const int scm_nBusAdapterInAdpNum = 1;
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_FB_DATA_ARRAY(2, 29, 2, 2)

  static const TForteUInt8 scm_slaveConfigurationIO[];
  static const TForteUInt8 scm_slaveConfigurationIO_num;

public:
  FUNCTION_BLOCK_CTOR_FOR_IO_MULTI_SLAVE(FORTE_IORevPiDIO, forte::core::IO::IOConfigFBMultiSlave, 96){
};

virtual ~FORTE_IORevPiDIO() {};

protected:

void initHandles();

};

#endif //close the ifdef sequence from the beginning of the file

