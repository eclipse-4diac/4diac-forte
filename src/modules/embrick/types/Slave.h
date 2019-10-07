/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#ifndef SRC_MODULES_EMBRICK_TYPES_SLAVE_H_
#define SRC_MODULES_EMBRICK_TYPES_SLAVE_H_

#include "BusAdapter.h"
#include <forte_sync.h>

#include <handler/bus.h>
#include <slave/slave.h>
#include <slave/handle.h>
#include "../../../core/io/configFB/io_slave_multi.h"

class EmbrickSlave : public forte::core::io::IOConfigFBMultiSlave, public EmbrickSlaveHandler::Delegate {
  public:
    EmbrickSlave(const TForteUInt8* const paSlaveConfigurationIO, const TForteUInt8 paSlaveConfigurationIO_num, int paType, CResource *paSrcRes,
        const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData);
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

    CSyncObject mSlaveMutex;
    EmbrickSlaveHandler *mSlave;

  public:
    void onSlaveStatus(EmbrickSlaveHandler::SlaveStatus paStatus, EmbrickSlaveHandler::SlaveStatus paOldStatus);
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
