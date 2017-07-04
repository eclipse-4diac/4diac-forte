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

#include <io/configFB/multi/io_slave.h>
#include "BusAdapter.h"
#include <forte_sync.h>

#include <handler/bus.h>
#include <slave/slave.h>
#include <slave/handle.h>

namespace EmBrick {
namespace FunctionBlocks {

class Slave: public IO::ConfigurationFB::Multi::Slave,
    public Handlers::Slave::Delegate {
public:
  Slave(const TForteUInt8* const scm_slaveConfigurationIO,
      const TForteUInt8 scm_slaveConfigurationIO_num, int type,
      CResource *pa_poSrcRes,
      const SFBInterfaceSpec *pa_pstInterfaceSpec,
      const CStringDictionary::TStringId pa_nInstanceNameId,
      TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData);
  virtual ~Slave();

protected:
  virtual CIEC_UINT &UpdateInterval() {
    // TODO Remove
    return *static_cast<CIEC_UINT*>(getDI(0));
  }

  BusAdapter& BusAdapterOut() {
    return (*static_cast<BusAdapter*>(m_apoAdapters[0]));
  }

  BusAdapter& BusAdapterIn() {
    return (*static_cast<BusAdapter*>(m_apoAdapters[1]));
  }

  CSyncObject slaveMutex;
  Handlers::Slave *slave;

  void addBitHandle(Mapper::Direction direction, CIEC_WSTRING id,
      uint8_t offset, uint8_t pos);
  void addAnalogHandle(Mapper::Direction direction, CIEC_WSTRING id,
      uint8_t offset);
  void addAnalog10Handle(Mapper::Direction direction, CIEC_WSTRING id,
      uint8_t offset);

public:
  void onSlaveStatus(Handlers::SlaveStatus status,
      Handlers::SlaveStatus oldStatus);
  void onSlaveDestroy();

private:
  const char* const init();
  void deInit();

  static const char * const scmSlow;
  static const char * const scmInterrupted;
  static const char * const scmError;
  static const char * const scmUnknown;
};

} /* namespace FunctionsBlocks */
} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_TYPES_SLAVE_H_ */
