/*******************************************************************************
 * Copyright (c) 2017 - 2018 fortiss GmbH
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

#ifndef SRC_CORE_IO_CONFIGFB_MASTER_MULTI_H_
#define SRC_CORE_IO_CONFIGFB_MASTER_MULTI_H_

#include "../device/io_controller_multi.h"
#include <fortelist.h>
#include "io_configFB_controller.h"
#include "io_adapter_multi.h"

namespace forte {
  namespace core {
    namespace io {

      class IOConfigFBMultiMaster;

      typedef CSinglyLinkedList<IOConfigFBMultiMaster*> TMasterList;

      class IOConfigFBMultiMaster : public IOConfigFBController {
        public:
          IOConfigFBMultiMaster(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId,
              TForteByte *paFBConnData, TForteByte *paFBVarsData);

          static IOConfigFBMultiMaster* getMasterById(TForteUInt16 paId);

          using IOConfigFBController::initHandle;

        protected:
          IOConfigFBMultiAdapter& BusAdapterOut() {
            return (*static_cast<IOConfigFBMultiAdapter*>(m_apoAdapters[0]));
          }
          static const int scmBusAdapterAdpNum = 0;

          virtual void onStartup();

          virtual void onStop();

          virtual void executeEvent(int paEIID);

        private:
          static TMasterList mInstances;

          static TForteUInt16 mInstancesIncrement;

          TForteUInt16 mId;

          static const char * const scmFailedToInitSlaves;
      };

    } //namespace IO
  } //namepsace core
} //namespace forte

#endif /* SRC_CORE_IO_CONFIGFB_MASTER_MULTI_H_ */
