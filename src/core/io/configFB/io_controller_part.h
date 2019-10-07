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

#ifndef SRC_CORE_IO_CONFIGFB_IO_CONTROLLER_PART_H_
#define SRC_CORE_IO_CONFIGFB_IO_CONTROLLER_PART_H_

#include "io_adapter_split.h"
#include "io_base.h"
#include "io_controller_split.h"

namespace forte {
  namespace core {
    namespace io {

      class IOConfigFBPartController : public IOConfigFBBase {
        public:
          IOConfigFBPartController(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId,
              TForteByte *paFBConnData, TForteByte *paFBVarsData);

        protected:
          CIEC_BOOL &QI() {
            return *static_cast<CIEC_BOOL*>(getDI(0));
          }

          CIEC_BOOL &QO() {
            return *static_cast<CIEC_BOOL*>(getDO(0));
          }

          static const TEventID scmEventMAPID = 0;

          static const TEventID scmEventMAPOID = 0;

          IOConfigFBSplitAdapter& IOConfigFBMultiAdapter() {
            return (*static_cast<IOConfigFBSplitAdapter*>(m_apoAdapters[0]));
          }

          static const int scmSplitAdapterAdpNum = 0;

          virtual void executeEvent(int paEIID);

          virtual void initHandles() = 0;

          void initHandle(IODeviceController::HandleDescriptor *paHandleDescriptor);

        private:
          IOConfigFBSplitController* mMaster;

      };

    } //namespace IO
  } //namepsace core
} //namespace forte

#endif /* SRC_CORE_IO_CONFIGFB_IO_CONTROLLER_PART_H_ */
