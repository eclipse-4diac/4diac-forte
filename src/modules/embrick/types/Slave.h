/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_MODULES_EMBRICK_TYPES_SLAVE_H_
#define SRC_MODULES_EMBRICK_TYPES_SLAVE_H_

#include "BusAdapter.h"
#include <forte_sync.h>

#include <handler/bus.h>
#include <slave/slave.h>
#include <slave/handle.h>
#include "../../../core/io/configFB/io_slave_multi.h"

class EmbrickSlave: public IOConfigFBMultiSlave,
    public EmbrickSlaveHandler::Delegate {
public:
  EmbrickSlave(const TForteUInt8* const scm_slaveConfigurationIO,
      const TForteUInt8 scm_slaveConfigurationIO_num, int type,
      CResource *pa_poSrcRes,
      const SFBInterfaceSpec *pa_pstInterfaceSpec,
      const CStringDictionary::TStringId pa_nInstanceNameId,
      TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData);
  virtual ~EmbrickSlave();

protected:
  virtual CIEC_UINT &UpdateInterval() {
    // TODO Remove
    return *static_cast<CIEC_UINT*>(getDI(0));
  }

  EmbrickBusAdapter& BusAdapterOut() {
    return (*static_cast<EmbrickBusAdapter*>(m_apoAdapters[0]));
  }

  EmbrickBusAdapter& BusAdapterIn() {
    return (*static_cast<EmbrickBusAdapter*>(m_apoAdapters[1]));
  }

  CSyncObject slaveMutex;
  EmbrickSlaveHandler *slave;

public:
  void onSlaveStatus(EmbrickSlaveHandler::SlaveStatus status,
      EmbrickSlaveHandler::SlaveStatus oldStatus);
  void onSlaveDestroy();

private:
  const char* init();
  void deInit();

  static const char * const scmSlow;
  static const char * const scmInterrupted;
  static const char * const scmError;
  static const char * const scmUnknown;
};


#endif /* SRC_MODULES_EMBRICK_TYPES_SLAVE_H_ */
