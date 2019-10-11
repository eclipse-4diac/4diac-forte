/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: SubscribeEventHTTP
 *** Description: Composite Function Block Type
 *** Version: 
 ***     1.0: 2019-09-18/Cabral - fortiss GmbH - 
 *************************************************************************/

#ifndef _SUBSCRIBEEVENTHTTP_H_
#define _SUBSCRIBEEVENTHTTP_H_

#include <cfb.h>
#include <typelib.h>
#include "SubscribeEventAdp.h"

class FORTE_SubscribeEventHTTP: public CCompositeFB{
  DECLARE_FIRMWARE_FB(FORTE_SubscribeEventHTTP)

private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const SAdapterInstanceDef scm_astAdapterInstances[];

  FORTE_SubscribeEventAdp& SubscribeEventAdp() {
    return (*static_cast<FORTE_SubscribeEventAdp*>(m_apoAdapters[0]));
  };
  static const int scm_nSubscribeEventAdpAdpNum = 0;
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(0, 0, 0, 1);

  static const SCFB_FBInstanceData scm_astInternalFBs[];

  static const SCFB_FBParameter scm_astParamters[];

  static const SCFB_FBConnectionData scm_astEventConnections[];

  static const SCFB_FBFannedOutConnectionData scm_astFannedOutEventConnections[];

  static const SCFB_FBConnectionData scm_astDataConnections[];

  static const SCFB_FBFannedOutConnectionData scm_astFannedOutDataConnections[];
  static const SCFB_FBNData scm_stFBNData;

public:
  COMPOSITE_FUNCTION_BLOCK_CTOR(FORTE_SubscribeEventHTTP){
  };

  virtual ~FORTE_SubscribeEventHTTP(){};

};

#endif //close the ifdef sequence from the beginning of the file

