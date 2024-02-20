/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: IORevPiAIO
 *** Description: Service Interface Function Block Type
 *** Version: 
 ***     0.0: 2017-10-26/4DIAC-IDE - 4DIAC-Consortium - 
 *************************************************************************/

#ifndef _IOREVPIAIO_H_
#define _IOREVPIAIO_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_wstring.h>
#include "IORevPiBusAdapter.h"
#include <io/configFB/io_slave_multi.h>

class FORTE_IORevPiAIO: public forte::core::IO::IOConfigFBMultiSlave {
  DECLARE_FIRMWARE_FB(FORTE_IORevPiAIO)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  };

  CIEC_WSTRING &AnalogInput_1() {
    return *static_cast<CIEC_WSTRING*>(getDI(1));
  };

  CIEC_WSTRING &AnalogInput_2() {
    return *static_cast<CIEC_WSTRING*>(getDI(2));
  };

  CIEC_WSTRING &AnalogInput_3() {
    return *static_cast<CIEC_WSTRING*>(getDI(3));
  };

  CIEC_WSTRING &AnalogInput_4() {
    return *static_cast<CIEC_WSTRING*>(getDI(4));
  };

  CIEC_WSTRING &RTD_1() {
    return *static_cast<CIEC_WSTRING*>(getDI(5));
  };

  CIEC_WSTRING &RTD_2() {
    return *static_cast<CIEC_WSTRING*>(getDI(6));
  };

  CIEC_WSTRING &AnalogOutput_1() {
    return *static_cast<CIEC_WSTRING*>(getDI(7));
  };

  CIEC_WSTRING &AnalogOutput_2() {
    return *static_cast<CIEC_WSTRING*>(getDI(8));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  };

  CIEC_WSTRING &STATUS() {
    return *static_cast<CIEC_WSTRING*>(getDO(1));
  };

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
  };
  static const int scm_nBusAdapterOutAdpNum = 0;
  FORTE_IORevPiBusAdapter& BusAdapterIn() {
    return (*static_cast<FORTE_IORevPiBusAdapter*>(m_apoAdapters[1]));
  };
  static const int scm_nBusAdapterInAdpNum = 1;
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(2, 9, 2, 2);

   static const TForteUInt8 scm_slaveConfigurationIO[];
   static const TForteUInt8 scm_slaveConfigurationIO_num;

public:
  FUNCTION_BLOCK_CTOR_FOR_IO_MULTI_SLAVE(FORTE_IORevPiAIO, forte::core::IO::IOConfigFBMultiSlave, 103){
};

  virtual ~FORTE_IORevPiAIO(){};

protected:
  void initHandles();

};

#endif //close the ifdef sequence from the beginning of the file

