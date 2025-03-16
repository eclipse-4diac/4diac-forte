/*******************************************************************************
 * Copyright (c) 2016, 2025 Johannes Messmer (admin@jomess.com), fortiss GmbH,
 *                          Thomas Öllinger, HR Agrartechnik GmbH,
 *                          Johannes Kepler University Linz
 * 
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral      - Cleaning of namespaces
 *   Thomas Öllinger  - added getId method to detremine Id for reference to I/O
 *                      configuration
 *   Alois ZOitl      - removed old process interface structure, added common
 *                      interface elements
 *******************************************************************************/

#pragma once

#include "mapper/io_mapper.h"
#include "mapper/io_observer.h"

#include <forte_sync.h>
#include "../esfb.h"

#include <string>

namespace forte::core::io {

    class CProcessInterfaceFB : public CEventSourceFB, public IOObserver {

      public:
        CProcessInterfaceFB(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec& paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId);
        ~CProcessInterfaceFB() override;

        bool onChange() override;

        EMGMResponse changeExecutionState(EMGMCommandType paCommand) override;

        CIEC_ANY *getDI(size_t) override;
        CIEC_ANY *getDO(size_t) override;

      protected:
        void readInputData(TEventID paEIID) override;
        void writeOutputData(TEventID paEIID) override;
        void setInitialValues() override;
        CEventConnection *getEOConUnchecked(TPortId) override;
        CDataConnection **getDIConUnchecked(TPortId) override;
        CDataConnection *getDOConUnchecked(TPortId) override;
        bool initialise(bool paIsInput, CEventChainExecutionThread *const paECET);
        bool deinitialise();

        CIEC_BOOL read(CIEC_ANY &paData);
        CIEC_BOOL write(const CIEC_ANY &paData);

        void onHandle(IOHandle* const paHandle) override;
        void dropHandle() override;

        CIEC_BOOL var_QI;
        CIEC_STRING var_PARAMS;

        CIEC_BOOL var_QO;
        CIEC_STRING var_STATUS;

        CIEC_BOOL var_conn_QO;
        CIEC_STRING var_conn_STATUS;

        CEventConnection conn_INITO;
        CEventConnection conn_CNF;

        CDataConnection *conn_QI;
        CDataConnection *conn_PARAMS;

        CDataConnection conn_QO;
        CDataConnection conn_STATUS;

      private:

        virtual CIEC_BOOL read() {
          return false_BOOL;
        }

        virtual CIEC_BOOL write() {
          return false_BOOL;
        }

        bool mIsListening;
        bool mIsReady;
        CSyncObject mSyncMutex;

        static const CIEC_STRING scmOK;
        static const CIEC_STRING scmWaitingForHandle;
        static const CIEC_STRING scmFailedToRegister;
        static const CIEC_STRING scmMappedWrongDirectionOutput;
        static const CIEC_STRING scmMappedWrongDirectionInput;
        static const CIEC_STRING scmMappedWrongDataType;

        static const TEventID scmEventINITID = 0;
        static const TEventID scmEventREQID = 1;
        static const TEventID scmEventINITOID = 0;
        static const TEventID scmEventCNFID = 1;

        std::string getId() const;
    };

}

