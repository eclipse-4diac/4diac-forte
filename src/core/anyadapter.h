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
    CAnyAdapter(CStringDictionary::TStringId paAdapterInstanceName, CResource *paSrcRes, bool paIsPlug);
    ~CAnyAdapter() override;

    void typifyAnyAdapter(CAdapter *paPeer);

    void setParentFB(CFunctionBlock *paParentFB, TForteUInt8 paParentAdapterlistID) override;

    bool disconnect(CAdapterConnection *paAdConn) override;

    //! Helper functions allowing to retrieve interface information from any_adpaters TODO look for Doxygen grouping syntax
    TEventID getNumEIs() const {
      return mInterfaceSpec->mNumEIs;
    }

    TEventID getNumEOs() const {
      return mInterfaceSpec->mNumEOs;
    }

    const TForteInt16* getEIWithIndexes() const {
      return mInterfaceSpec->mEIWithIndexes;
    }

    const TDataIOID* getEIWiths() const {
      return mInterfaceSpec->mEIWith;
    }

    TPortId getNumDIs() const {
      return mInterfaceSpec->mNumDIs;
    }

    const CStringDictionary::TStringId* getDataInputNames() const {
      return mInterfaceSpec->mDINames;
    }

    TIEC_ANYPtr getDataInputs(){
      return getDI(0);
    }

    TPortId getNumDOs() const {
      return mInterfaceSpec->mNumDOs;
    }

    const CStringDictionary::TStringId* getDataOutputNames() const {
      return mInterfaceSpec->mDONames;
    }

    TIEC_ANYPtr getDataOutputs(){
      return getDO(0);
    }

  protected:
    void readInputData(TEventID) override {
    }

    void writeOutputData(TEventID) override {
    }

  private:
    static const SFBInterfaceSpec scmFBInterfaceSpec; //! interface spec for the empty interface of an any adapter will be used for plug and socket

    //!Interface specification to be used when configured
    SFBInterfaceSpec mCurrentFBInterfaceSpec;

    CFunctionBlock *m_ParentFB; //!< Pointer to the parent FB
    TForteUInt8 mParentAdapterlistID; //!< Adapter list ID in respect to the parent FB
};

#endif /* ANYADAPTER_H_ */
