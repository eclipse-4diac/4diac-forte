/*******************************************************************************
 * Copyright (c) 2006, 2023 ACIN, Profactor GmbH, fortiss GmbH
 *                          Johannes Kepler University
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Rene Smodic, Thomas Strasser, Alois Zoitl, Michael Hofmann,
 *    Martin Melik-Merkumians, Ingo Hegny, Patrick Smejkal
 *      - initial implementation and rework communication infrastructure
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *    Martin Jobst - add generic readInputData and writeOutputData
 *******************************************************************************/
#ifndef _COMMFB_H_
#define _COMMFB_H_

#include "forte/config/forte_config.h"
#include "forte/cominfra/basecommfb.h"
#include "forte/cominfra/comtypes.h"

namespace forte {
  namespace com_infra {

    class CCommFB : public CBaseCommFB {
      public:
        ~CCommFB() override;

        EMGMResponse changeExecutionState(EMGMCommandType paCommand) override;

        CIEC_BOOL var_QI;
        CIEC_WSTRING var_ID;

        CIEC_BOOL var_QO;
        CIEC_WSTRING var_STATUS;

        CEventConnection conn_INITO;
        CEventConnection conn_CNF_IND;

        CDataConnection *conn_QI;
        CDataConnection *conn_ID;

        COutDataConnection<CIEC_BOOL> conn_QO;
        COutDataConnection<CIEC_WSTRING> conn_STATUS;

        CIEC_ANY **getSDs() override {
          return mGenDIs.get();
        }

        CIEC_ANY **getRDs() override {
          return mGenDOs.get();
        }

      protected:
        CCommFB(const StringId paInstanceNameId, CFBContainer &paContainer, EComServiceType paCommServiceType);

        void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

        void readInputData(TEventID paEI) override;
        void writeOutputData(TEventID paEO) override;

        static const TEventID scmEventINITID = 0;
        static const TEventID scmEventINITOID = 0;
        static const TEventID scmSendNotificationEventID = 1;
        static const TEventID scmReceiveNotificationEventID = 1;

        static const char *const scmDefaultIDPrefix;
        static const char *const scmDefaultIDSuffix;

        static char *extractLayerIdAndParams(char **paRemainingID, char **paLayerParams);

        char *getDefaultIDString(const char *paID) override;

        EComResponse receiveData() override;
        EComResponse sendData() override;

        size_t getGenEOOffset() override {
          return 2;
        }

        size_t getGenDIOffset() override {
          return 2;
        }

        size_t getGenDOOffset() override {
          return 2;
        }

        CIEC_ANY *getDI(size_t) override;
        CIEC_ANY *getDO(size_t) override;
        CEventConnection *getEOConUnchecked(TPortId) override;
        CDataConnection **getDIConUnchecked(TPortId) override;
        CDataConnection *getDOConUnchecked(TPortId) override;

        void createGenInputData() override;
        void createGenOutputData() override;

        std::unique_ptr<CIEC_ANY *[]> mGenDIs;
        std::unique_ptr<CIEC_ANY *[]> mGenDOs;

      private:
        std::vector<StringId> mDiNames;
        std::vector<StringId> mDoNames;

        bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

        void configureDIs(const char *paDIConfigString, SFBInterfaceSpec &paInterfaceSpec);
        void configureDOs(const char *paDOConfigString, SFBInterfaceSpec &paInterfaceSpec);
    };

  } // namespace com_infra
} // namespace forte

#endif //_COMMFB_H_
