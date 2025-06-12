/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH, fortiss GmbH
 *                      2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *    Martin Jobst - add generic readInputData and writeOutputData
 *******************************************************************************/
#ifndef _GEN_VALUES2ARRAY_H_
#define _GEN_VALUES2ARRAY_H_

#include <genfb.h>

#include <memory>
#include "forte_any_variant.h"

class GEN_VALUES2ARRAY final : public CGenFunctionBlock<CFunctionBlock> {
    DECLARE_GENERIC_FIRMWARE_FB(GEN_VALUES2ARRAY)

  protected:
    size_t getGenEOOffset() override {
      return 1;
    }

    size_t getGenDOOffset() override {
      return 1;
    }

    void createGenInputData() override;

    CEventConnection *getEOConUnchecked(TPortId paEONum) override;
    CIEC_ANY *getDI(size_t paIndex) override;
    CIEC_ANY *getDO(size_t paIndex) override;
    CDataConnection *getDOConUnchecked(TPortId paDONum) override;

  private:
    static const TEventID scmEventREQID = 0;

    static const TEventID scmEventCNFID = 0;

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEI) override;
    void writeOutputData(TEventID paEO) override;

    bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

    std::vector<CStringDictionary::TStringId> mDataInputNames;
    CEventConnection conn_CNF;
    CIEC_ARRAY_DYNAMIC var_OUT;
    COutDataConnection<CIEC_ARRAY_DYNAMIC> conn_OUT;
    std::unique_ptr<CIEC_ANY_VARIANT[]> mGenDIs;

  public:
    GEN_VALUES2ARRAY(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);
    ~GEN_VALUES2ARRAY() override = default;
};

#endif //_GEN_VALUES2ARRAY_H_
