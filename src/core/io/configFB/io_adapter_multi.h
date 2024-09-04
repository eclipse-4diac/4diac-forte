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

#include <adapter.h>

namespace forte {
  namespace core {
    namespace io {

#define ADAPTER_CTOR_FOR_IO_MULTI(fbclass) \
 fbclass(CStringDictionary::TStringId paAdapterInstanceName, forte::core::CFBContainer &paContainer, bool paIsPlug) : \
 IOConfigFBMultiAdapter( scmSlaveConfigurationIO, scmSlaveConfigurationIONum, paContainer, scmFBInterfaceSpecSocket, paAdapterInstanceName, scmFBInterfaceSpecPlug, paIsPlug)

      class IOConfigFBMultiAdapter : public CAdapter {
        public:
          IOConfigFBMultiAdapter(const TForteUInt8* const paSlaveConfigurationIO, const TForteUInt8 paSlaveConfigurationIONum, forte::core::CFBContainer &paContainer,
              const SFBInterfaceSpec &patInterfaceSpecSocket, const CStringDictionary::TStringId paInstanceNameId, const SFBInterfaceSpec& paInterfaceSpecPlug,
              bool paIsPlug);
          ~IOConfigFBMultiAdapter() override;

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

          static const TEventID scmEventINITID = 0;
          TEventID INIT() const {
            return mParentAdapterListEventID + scmEventINITID;
          }

          static const TEventID scmEventINITOID = 0;
          TEventID INITO() const {
            return mParentAdapterListEventID + scmEventINITOID;
          }

          const TForteUInt8* cmSlaveConfigurationIO;
          TForteUInt8 mSlaveConfigurationIONum;

          CIEC_ANY* getSlaveConfig(int paIndex) {
            return (isSocket()) ? getDO(cmSlaveConfigurationIO[paIndex]) : getDI(cmSlaveConfigurationIO[paIndex]);
          }

      };

    } //namespace IO
  } //namepsace core
} //namespace forte

#endif /* SRC_CORE_IO_CONFIGFB_ADAPTER_MULTI_H_ */
