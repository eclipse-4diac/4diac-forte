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
 fbclass(CStringDictionary::TStringId paAdapterInstanceName, CResource *paSrcRes, bool paIsPlug) : \
 IOConfigFBMultiAdapter( (const TForteUInt8* const) &scmSlaveConfigurationIO, scmSlaveConfigurationIONum, paSrcRes, &scm_stFBInterfaceSpecSocket, paAdapterInstanceName, &scm_stFBInterfaceSpecPlug, paIsPlug, m_anFBConnData, m_anFBVarsData)

      class IOConfigFBMultiAdapter : public CAdapter {
        public:
          IOConfigFBMultiAdapter(const TForteUInt8* const paSlaveConfigurationIO, const TForteUInt8 paSlaveConfigurationIONum, CResource *paSrcRes,
              const SFBInterfaceSpec *patInterfaceSpecSocket, const CStringDictionary::TStringId paInstanceNameId, const SFBInterfaceSpec *paInterfaceSpecPlug,
              bool paIsPlug, TForteByte *paFBConnData, TForteByte *paFBVarsData);
          virtual ~IOConfigFBMultiAdapter();

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
          int INIT() const {
            return m_nParentAdapterListEventID + scmEventINITID;
          }

          static const TEventID scmEventINITOID = 0;
          int INITO() const {
            return m_nParentAdapterListEventID + scmEventINITOID;
          }

          const TForteUInt8* cmSlaveConfigurationIO;
          TForteUInt8 mSlaveConfigurationIONum;

          TIEC_ANYPtr getSlaveConfig(int paIndex) {
            return (isSocket()) ? getDO(cmSlaveConfigurationIO[paIndex]) : getDI(cmSlaveConfigurationIO[paIndex]);
          }

      };

    } //namespace IO
  } //namepsace core
} //namespace forte

#endif /* SRC_CORE_IO_CONFIGFB_ADAPTER_MULTI_H_ */
