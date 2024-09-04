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

#ifndef SRC_CORE_IO_CONFIGFB_IO_CONTROLLER_SPLIT_H_
#define SRC_CORE_IO_CONFIGFB_IO_CONTROLLER_SPLIT_H_

#include "io_configFB_controller.h"
#include "io_adapter_split.h"

namespace forte {
  namespace core {
    namespace io {

#define FUNCTION_BLOCK_CTOR_FOR_IO_SPLIT_CONTROLLER(fbclass) \
 fbclass(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) : \
 IOConfigFBSplitController( (const TForteUInt8* const) &scmSplitAdapter, scmSplitAdapterNum, paContainer, scmFBInterfaceSpec, paInstanceNameId)

      class IOConfigFBSplitController;

      typedef CSinglyLinkedList<IOConfigFBSplitController*> TControllerList;

      class IOConfigFBSplitController : public IOConfigFBController {
        public:
          IOConfigFBSplitController(const TForteUInt8* const paSplitAdapter, const TForteUInt8 paSplitAdapterNum, forte::core::CFBContainer &paContainer,
              const SFBInterfaceSpec& paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId);

          static IOConfigFBSplitController* getControllerById(TForteUInt16 paId);

          using IOConfigFBController::initHandle;

        protected:

          void onStartup(CEventChainExecutionThread * const paECET) override;

          void onStop(CEventChainExecutionThread * const paECET) override;

          void executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET) override;

        private:
          const TForteUInt8* scmSplitAdapter;
          const TForteUInt8 scmSplitAdapterNum;

          TForteUInt8 mSplitIterator;

          static TControllerList mInstances;

          static TForteUInt16 mInstancesIncrement;

          TForteUInt16 mId;

          static const char * const scmFailedToInitParts;
      };

    } //namespace IO
  } //namepsace core
} //namespace forte

#endif /* SRC_CORE_IO_CONFIGFB_IO_CONTROLLER_SPLIT_H_ */
