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

#ifndef SRC_CORE_IO_CONFIGFB_MULTI_ADAPTER_H_
#define SRC_CORE_IO_CONFIGFB_MULTI_ADAPTER_H_

#include <adapter.h>

#define ADAPTER_CTOR_FOR_IO_MULTI(fbclass) \
 fbclass(CStringDictionary::TStringId pa_anAdapterInstanceName, CResource *pa_poSrcRes, bool pa_bIsPlug) : \
 IO::ConfigurationFB::Multi::Adapter( (const TForteUInt8* const) &scm_slaveConfigurationIO, scm_slaveConfigurationIO_num, pa_poSrcRes, &scm_stFBInterfaceSpecSocket, pa_anAdapterInstanceName, &scm_stFBInterfaceSpecPlug, pa_bIsPlug, m_anFBConnData, m_anFBVarsData)

namespace IO {
namespace ConfigurationFB {
namespace Multi {

class Adapter: public CAdapter {
public:
  Adapter(const TForteUInt8* const scm_slaveConfigurationIO,
      const TForteUInt8 scm_slaveConfigurationIO_num, CResource *pa_poSrcRes,
      const SFBInterfaceSpec *pa_pstInterfaceSpecSocket,
      const CStringDictionary::TStringId pa_nInstanceNameId,
      const SFBInterfaceSpec *pa_pstInterfaceSpecPlug, bool pa_bIsPlug,
      TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData);
  virtual ~Adapter();

  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>((isSocket()) ? getDI(0) : getDO(0));
  }

  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>((isSocket()) ? getDO(0) : getDI(0));
  }

  CIEC_UINT &MasterId() {
    return *static_cast<CIEC_UINT*>((isSocket()) ? getDO(1) : getDI(1));
  }

  CIEC_UINT &Index() {
    return *static_cast<CIEC_UINT*>((isSocket()) ? getDO(2) : getDI(2));
  }

  static const TEventID scm_nEventINITID = 0;
  int INIT() {
    return m_nParentAdapterListEventID + scm_nEventINITID;
  }

  static const TEventID scm_nEventINITOID = 0;
  int INITO() {
    return m_nParentAdapterListEventID + scm_nEventINITOID;
  }

  const TForteUInt8* scm_slaveConfigurationIO;
  TForteUInt8 scm_slaveConfigurationIO_num;

  TIEC_ANYPtr getSlaveConfig(int index) {
    return
        (isSocket()) ?
            getDO(scm_slaveConfigurationIO[index]) :
            getDI(scm_slaveConfigurationIO[index]);
  }
};

} /* namespace Multi */
} /* namespace ConfigurationFB */
} /* namespace IO */

#endif /* SRC_CORE_IO_CONFIGFB_MULTI_ADAPTER_H_ */
