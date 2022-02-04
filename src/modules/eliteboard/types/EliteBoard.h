/*******************************************************************************
 * Copyright (c) 2021, 2022 Jonathan Lainer (kontakt@lainer.co.at)
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *******************************************************************************/

#ifndef _ELITEBOARD_H_
#define _ELITEBOARD_H_

#include "core/io/configFB/io_configFB_controller.h"
#include "PortAdapter.h"
#include "forte_array_at.h"
#include "funcbloc.h"
#include <extevhandlerhelper.h>
#include <handler/EliteBoardDeviceController.h>

class FORTE_EliteBoard : public forte::core::io::IOConfigFBController {
  DECLARE_FIRMWARE_FB(FORTE_EliteBoard)

private:
  static const TEventID scm_nEventMAPID = 0;

  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventMAPOID = 0;

  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const int scm_nPortAAdpNum = 0;
  static const int scm_nPortBAdpNum = 1;
  static const int scm_nPortCAdpNum = 2;
  static const int scm_nPortDAdpNum = 3;
  static const int scm_nPortEAdpNum = 4;
  static const int scm_nPortFAdpNum = 5;
  static const int scm_nPortGAdpNum = 6;
  static const int scm_nPortHAdpNum = 7;
  static const int scm_nPortIAdpNum = 8;
  static const int scm_nPortJAdpNum = 9;
  static const int scm_nPortKAdpNum = 10;

  static const SAdapterInstanceDef scm_astAdapterInstances[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_PortAdapter &st_PortA() {
    return (*static_cast<FORTE_PortAdapter *>(m_apoAdapters[0]));
  };

  FORTE_PortAdapter &st_PortB() {
    return (*static_cast<FORTE_PortAdapter *>(m_apoAdapters[1]));
  };

  FORTE_PortAdapter &st_PortC() {
    return (*static_cast<FORTE_PortAdapter *>(m_apoAdapters[2]));
  };

  FORTE_PortAdapter &st_PortD() {
    return (*static_cast<FORTE_PortAdapter *>(m_apoAdapters[3]));
  };

  FORTE_PortAdapter &st_PortE() {
    return (*static_cast<FORTE_PortAdapter *>(m_apoAdapters[4]));
  };

  FORTE_PortAdapter &st_PortF() {
    return (*static_cast<FORTE_PortAdapter *>(m_apoAdapters[5]));
  };

  FORTE_PortAdapter &st_PortG() {
    return (*static_cast<FORTE_PortAdapter *>(m_apoAdapters[6]));
  };

  FORTE_PortAdapter &st_PortH() {
    return (*static_cast<FORTE_PortAdapter *>(m_apoAdapters[7]));
  };

  FORTE_PortAdapter &st_PortI() {
    return (*static_cast<FORTE_PortAdapter *>(m_apoAdapters[8]));
  };

  FORTE_PortAdapter &st_PortJ() {
    return (*static_cast<FORTE_PortAdapter *>(m_apoAdapters[9]));
  };

  FORTE_PortAdapter &st_PortK() {
    return (*static_cast<FORTE_PortAdapter *>(m_apoAdapters[10]));
  };

  FORTE_FB_DATA_ARRAY(1, 0, 0, 11);

  void executeEvent(int pa_nEIID);
 
  void setConfig() {}
  
  EliteBoardDeviceController& mEventHandler;
  int mCurrentAdapterIndex = 0;
  static const int mAdapterCount = scm_nPortKAdpNum + 1;

  forte::core::io::IODeviceController *
  createDeviceController(CDeviceExecution &paDeviceExecution);
  FORTE_PortAdapter &getPortAdapterByIndex(int index);
  bool configurePortFB(int index);

public:

  FORTE_EliteBoard(const CStringDictionary::TStringId pa_nInstanceNameId, 
      CResource *pa_poSrcRes);

  virtual ~FORTE_EliteBoard() = default;
};

#endif // _ELITEBOARD_H_
