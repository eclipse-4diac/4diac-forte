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

#ifndef SRC_CORE_IO_CONFIGFB_SLAVE_MULTI_H_
#define SRC_CORE_IO_CONFIGFB_SLAVE_MULTI_H_

#include "io_base.h"
#include <io/device/io_controller_multi.h>
#include "io_adapter_multi.h"
#include "io_master_multi.h"


namespace forte {
  namespace core {
    namespace IO {

#define FUNCTION_BLOCK_CTOR_FOR_IO_MULTI_SLAVE(fbclass, fbBaseClass, type) \
 fbclass(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : \
 fbBaseClass( (const TForteUInt8* const) &scm_slaveConfigurationIO, scm_slaveConfigurationIO_num, type, pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData)

class IOConfigFBMultiSlave: public IOConfigFBBase {
public:
  IOConfigFBMultiSlave(const TForteUInt8* const scm_slaveConfigurationIO,
      const TForteUInt8 scm_slaveConfigurationIO_num, int type,
      CResource *pa_poSrcRes,
      const SFBInterfaceSpec *pa_pstInterfaceSpec,
      const CStringDictionary::TStringId pa_nInstanceNameId,
      TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData);
  virtual ~IOConfigFBMultiSlave();

protected:
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  }

  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  }

  CIEC_WSTRING &STATUS() {
    return *static_cast<CIEC_WSTRING*>(getDO(1));
  }

  static const TEventID scm_nEventMAPID = 0;

  static const TEventID scm_nEventMAPOID = 0;
  static const TEventID scm_nEventINDID = 1;

  IOConfigFBMultiAdapter& BusAdapterOut() {
    return (*static_cast<IOConfigFBMultiAdapter*>(m_apoAdapters[0]));
  }

  static const int scm_nBusAdapterOutAdpNum = 0;
  IOConfigFBMultiAdapter& BusAdapterIn() {
    return (*static_cast<IOConfigFBMultiAdapter*>(m_apoAdapters[1]));
  }

  static const int scm_nBusAdapterInAdpNum = 1;

  virtual void executeEvent(int pa_nEIID);

  IOConfigFBMultiMaster* master;

  int index;

  int type;

  IODeviceMultiController& getController() {
    return (*static_cast<IODeviceMultiController*>(master->getDeviceController()));
  }

  bool initialized;

  virtual const char* init() {
    return 0;
  }

  virtual void deInit() {

  }

  virtual void initHandles() = 0;

  void initHandle(IODeviceMultiController::HandleDescriptor *handleDescriptor);

  static const char* const scmOK;
  static const char* const scmMasterNotFound;

  const TForteUInt8* scm_slaveConfigurationIO;
  TForteUInt8 scm_slaveConfigurationIO_num;
  bool* scm_slaveConfigurationIO_isDefault;

private:
  const char* handleInitEvent();

  static const char* const scmStopped;
  static const char* const scmNotFound;
  static const char* const scmIncorrectType;
};

    } //namespace IO
  } //namepsace core
} //namespace forte

#endif /* SRC_CORE_IO_CONFIGFB_SLAVE_MULTI_H_ */
