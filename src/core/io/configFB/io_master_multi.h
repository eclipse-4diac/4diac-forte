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

#include "io_configFB_controller.h"
#include "io_adapter_multi.h"

namespace forte {
  namespace core {
    namespace io {

      class IOConfigFBMultiMaster;

      class IOConfigFBMultiMaster : public IOConfigFBController {
        public:
          IOConfigFBMultiMaster(forte::core::CFBContainer &paContainer,
                                const SFBInterfaceSpec &paInterfaceSpec,
                                const CStringDictionary::TStringId paInstanceNameId);

          static IOConfigFBMultiMaster *getMasterById(TForteUInt16 paId);

          using IOConfigFBController::initHandle;

        protected:
          IOConfigFBMultiAdapter &BusAdapterOut() {
            return (*static_cast<IOConfigFBMultiAdapter *>(getPlugPinUnchecked(0)->getAdapterBlock()));
          }

          void onStartup(CEventChainExecutionThread *const paECET) override;

          void onStop(CEventChainExecutionThread *const paECET) override;

          void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

        private:
          static std::vector<IOConfigFBMultiMaster *> mInstances;

          static TForteUInt16 mInstancesIncrement;

          TForteUInt16 mId;

          static const char *const scmFailedToInitSlaves;
      };

    } // namespace io
  } // namespace core
} // namespace forte

#endif /* SRC_CORE_IO_CONFIGFB_MASTER_MULTI_H_ */
