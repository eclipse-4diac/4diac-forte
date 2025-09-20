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

#ifndef SRC_CORE_IO_CONFIGFB_ADAPTER_MULTI_H_
#define SRC_CORE_IO_CONFIGFB_ADAPTER_MULTI_H_

#include "forte/adapter.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_uint.h"

namespace forte::io {

  class IOConfigFBMultiAdapter : public CAdapter {
    public:
      IOConfigFBMultiAdapter(const std::span<const TForteUInt8> paSlaveConfigurationIO,
                             CFBContainer &paContainer,
                             const SFBInterfaceSpec &paInterfaceSpec,
                             const StringId paInstanceNameId,
                             TForteUInt8 paParentAdapterlistID);
      ~IOConfigFBMultiAdapter() override;

      void setInitialValues() override;

      CIEC_BOOL var_QO;

      CIEC_BOOL var_QI;

      CIEC_UINT var_MasterId;

      CIEC_UINT var_Index;

      static const TEventID scmEventINITID = 0;
      TEventID INIT() const {
        return getParentAdapterListEventID() + scmEventINITID;
      }

      static const TEventID scmEventINITOID = 0;
      TEventID INITO() const {
        return getParentAdapterListEventID() + scmEventINITOID;
      }

      const std::span<const TForteUInt8> cmSlaveConfigurationIO;

      virtual CIEC_ANY *getDeviceConfigPin(int paIndex) = 0;
  };

} // namespace forte::io

#endif /* SRC_CORE_IO_CONFIGFB_ADAPTER_MULTI_H_ */
