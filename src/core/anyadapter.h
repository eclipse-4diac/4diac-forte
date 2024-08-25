/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH, 2018 TU Vienna/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians
 *      - fixes event chain initialisation, adds typifyAnyAdapter
 *******************************************************************************/
#ifndef ANYADAPTER_H_
#define ANYADAPTER_H_

#include "adapter.h"

/*! \brief A generic adapter that behaves similar to the ANY data type and allows all other adapters to be connected.
 *
 *  The main use of such an adapter is in service interfaces that provide generic services based on the adapter connected.
 */

class CAnyAdapter : public CAdapter{
  DECLARE_ADAPTER_TYPE(CAnyAdapter)

  public:
    CAnyAdapter(CStringDictionary::TStringId paAdapterInstanceName, forte::core::CFBContainer &paContainer, bool paIsPlug);
    ~CAnyAdapter() override;

    void typifyAnyAdapter(CAdapter *paPeer);

    void setParentFB(CFunctionBlock *paParentFB, TForteUInt8 paParentAdapterlistID) override;

    bool disconnect(CAdapterConnection *paAdConn) override;

    //! Helper functions allowing to retrieve interface information from any_adpaters TODO look for Doxygen grouping syntax
    TEventID getNumEIs() const {
      return getFBInterfaceSpec().mNumEIs;
    }

    TEventID getNumEOs() const {
      return getFBInterfaceSpec().mNumEOs;
    }

    const TForteInt16* getEIWithIndexes() const {
      return getFBInterfaceSpec().mEIWithIndexes;
    }

    const TDataIOID* getEIWiths() const {
      return getFBInterfaceSpec().mEIWith;
    }

    TPortId getNumDIs() const {
      return getFBInterfaceSpec().mNumDIs;
    }

    const CStringDictionary::TStringId* getDataInputNames() const {
      return getFBInterfaceSpec().mDINames;
    }

    CIEC_ANY* getDataInputs(){
      return getDI(0);
    }

    TPortId getNumDOs() const {
      return getFBInterfaceSpec().mNumDOs;
    }

    const CStringDictionary::TStringId* getDataOutputNames() const {
      return getFBInterfaceSpec().mDONames;
    }

    CIEC_ANY* getDataOutputs(){
      return getDO(0);
    }

  protected:
    void readInputData(TEventID) override {
    }

    void writeOutputData(TEventID) override {
    }

  private:
    static const SFBInterfaceSpec scmFBInterfaceSpec; //! interface spec for the empty interface of an any adapter will be used for plug and socket

    CFunctionBlock *m_ParentFB; //!< Pointer to the parent FB
    TForteUInt8 mParentAdapterlistID; //!< Adapter list ID in respect to the parent FB
};

#endif /* ANYADAPTER_H_ */
