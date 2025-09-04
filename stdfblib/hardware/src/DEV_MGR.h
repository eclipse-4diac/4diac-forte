/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Rene Smodic, Thomas Strasser, Gerhard Ebenhofer,
 *   Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _DEV_MGR_H_
#define _DEV_MGR_H_

#include "forte/funcbloc.h"
#include "forte/mgmcmdstruct.h"
#include "forte/cominfra/commfb.h"
#include "CommandParser.h"
#include "forte/datatypes/forte_string.h"

class CDevice;

/*! \brief Implementation of the DEV_MGR FB.
 */
class DEV_MGR : public forte::com_infra::CCommFB {
    DECLARE_FIRMWARE_FB(DEV_MGR)

  public:
    DEV_MGR(forte::core::StringId paInstanceNameId, forte::core::CFBContainer &paContainer);
    ~DEV_MGR() override;

    bool initialize() override;

  protected:
    void createGenInputData() override;
    void createGenOutputData() override;

  private:
    bool executeCommand(const char *const paDest, char *paCommand);

    EMGMResponse parseAndExecuteMGMCommand(const char *const paDest, char *paCommand);

    static const TEventID scmEventINITID = 0;
    static const TEventID scmEventREQID = 1;

    static const TEventID scmEventINITOID = 0;
    static const TEventID scmEventCNFID = 1;

    //! The device the block is contained in
    CDevice &mDevice;

    forte::hardware::CommandParser mCommandParser;

    void executeRQST();

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    CIEC_BOOL &QI() {
      return *static_cast<CIEC_BOOL *>(getDI(0));
    };

    CIEC_STRING &ID() {
      return *static_cast<CIEC_STRING *>(getDI(1));
    };

    CIEC_STRING &RESP() {
      return *static_cast<CIEC_STRING *>(getDI(2));
    };

    CIEC_BOOL &QO() {
      return *static_cast<CIEC_BOOL *>(getDO(0));
    };

    CIEC_STRING &STATUS() {
      return *static_cast<CIEC_STRING *>(getDO(1));
    };

    CIEC_STRING &DST() {
      return *static_cast<CIEC_STRING *>(getDO(2));
    };

    CIEC_STRING &RQST() {
      return *static_cast<CIEC_STRING *>(getDO(3));
    };
};

#endif /*DEV_MGR_H_*/
