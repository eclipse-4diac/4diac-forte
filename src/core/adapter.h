/*******************************************************************************
 * Copyright (c) 2008 - 2015 ACIN, fortiss GmbH, 2018 TU Vienna/ACIN
 *               2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ingo Hegny, Alois Zoitl, Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - fixes connect, prepares for working AnyAdapter
 *    Martin Jobst - account for data type size in FB initialization
 *******************************************************************************/
#ifndef _ADAPTER_H_
#define _ADAPTER_H_

#include "funcbloc.h"

class CAdapterConnection;

#define ADAPTER_CTOR(fbclass) \
 fbclass(CStringDictionary::TStringId paAdapterInstanceName, CResource *paSrcRes, bool paIsPlug) : \
 CAdapter( paSrcRes, &scmFBInterfaceSpecSocket, paAdapterInstanceName, &scmFBInterfaceSpecPlug, paIsPlug)

#define ADAPTER_CTOR_WITH_BASE_CLASS(fbclass, fbBaseClass) \
 fbclass(CStringDictionary::TStringId paAdapterInstanceName, CResource *paSrcRes, bool paIsPlug) : \
 fbBaseClass( paSrcRes, &scmFBInterfaceSpecSocket, paAdapterInstanceName, &scmFBInterfaceSpecPlug, paIsPlug)

/*!\ingroup CORE\brief Class for handling adapters.
 *
 */
class CAdapter : public CFunctionBlock{
  public:
    /*!\brief The main constructor for an adapter instance.
     */
    //TODO: think on interface
    CAdapter(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpecSocket, const CStringDictionary::TStringId paInstanceNameId, const SFBInterfaceSpec *paInterfaceSpecPlug, bool paIsPlug);
    ~CAdapter() override;

    bool initialize() override;

    /*!\brief Returns if Adapter instance is a Plug
     */
    bool isPlug() const{
      return mIsPlug;
    }

    /*!\brief Returns if Adapter instance is a Socket
     */
    bool isSocket() const{
      return !mIsPlug;
    }

    virtual void setParentFB(CFunctionBlock *paParentFB, TForteUInt8 paParentAdapterlistID);

    /*! \brief Connects adapter to peer adapter
     *
     *   With this command the adapter is connected to a complementary adapter instance (peer).
     *   The data output pointers are "redirected" to the data inputs of the peer.
     *   \param paPeer  Pointer to the peer to connect to.
     *   \param paAdConn Pointer to the connecting adapter connection.
     *   \return success of establishment of connection
     */
    bool connect(CAdapter *paPeer, CAdapterConnection *paAdConn);

    /*! \brief Disconnects adapter from peer adapter
     *
     *   With this command the adapter is disconnected from a complementary adapter instance (peer).
     *   The redirection of data output pointers is removed.
     *   \param paAdConn Pointer to the connecting adapter connection.
     *   \return success of disconnection
     */
    virtual bool disconnect(CAdapterConnection *paAdConn = nullptr);

    /*! \brief Returns the compatibility of the adapter to another adapter
     *
     *   This method will evaluate the compatibility of this adapter to another one.
     *   Only instances of compatible adapter types may be interconnected.
     *   \param paPeer Pointer to a potential peer, whose compatibility has to be checked.
     *   \return compatibility status
     */
    bool isCompatible(CAdapter *paPeer) const;

    /*! provides access to the interface spec used for the specific adatpter instnace (i.e., plug or socket)
     *  Is required mainly by the anyadapter
     */
    const SFBInterfaceSpec* getAdapterInterfaceSpec() const{
      return mInterfaceSpec;
    }

    const TForteInt16 *getEventInputWithIndices() const{
      return getAdapterInterfaceSpec()->mEIWithIndexes;
    }

    const TForteInt16 *getEventOutputWithIndices() const{
      return getAdapterInterfaceSpec()->mEOWithIndexes;
    }

    const TDataIOID *getEventInputWith() const{
      return getAdapterInterfaceSpec()->mEIWith;
    }

    const TDataIOID *getEventOutputWith() const{
      return getAdapterInterfaceSpec()->mEOWith;
    }

    CAdapter *getPeer(){
      return mPeer;
    }

    CAdapterConnection *getAdapterConnection() const{
      return mAdapterConn;
    }

  protected:
    /**!
     *  @brief fills the event entry list of an adapter
     *
     *  @param paParentFB The FB which shall be entered into the event entry list
     */
    void fillEventEntryList(CFunctionBlock* paParentFB);

    TForteUInt16 mParentAdapterListEventID;

  private:
    void executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET) override;
    void setupEventEntryList();

    const bool mIsPlug;
    CAdapter *mPeer;
    CIEC_ANY **mLocalDIs;
    CAdapterConnection *mAdapterConn;
    TEventEntry *mEventEntry; //! the event entry list to start the event chain
};

#endif /*_ADAPTER_H_*/
