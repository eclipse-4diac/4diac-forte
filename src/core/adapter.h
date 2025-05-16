/*******************************************************************************
 * Copyright (c) 2008, 2025 ACIN, fortiss GmbH, 2018 TU Vienna/ACIN,
 *                          Martin Erich Jobst
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
#pragma once

#include "genfb.h"

class CAdapterConnection;

#define ADAPTER_CTOR(fbclass)                                                                                          \
  fbclass(CStringDictionary::TStringId paAdapterInstanceName, forte::core::CFBContainer &paContainer, bool paIsPlug) : \
      CAdapter(paContainer, &scmFBInterfaceSpecSocket, paAdapterInstanceName, &scmFBInterfaceSpecPlug, paIsPlug)

#define ADAPTER_CTOR_WITH_BASE_CLASS(fbclass, fbBaseClass)                                                             \
  fbclass(CStringDictionary::TStringId paAdapterInstanceName, forte::core::CFBContainer &paContainer, bool paIsPlug) : \
      fbBaseClass(paContainer, &scmFBInterfaceSpecSocket, paAdapterInstanceName, &scmFBInterfaceSpecPlug, paIsPlug)

/*!\ingroup CORE\brief Class for handling adapters.
 *
 */
class CAdapter : public CGenFunctionBlock<CFunctionBlock> {
  public:
    /*!\brief The main constructor for an adapter instance.
     */
    // TODO: think on interface
    CAdapter(forte::core::CFBContainer &paContainer,
             const SFBInterfaceSpec &paInterfaceSpecSocket,
             const CStringDictionary::TStringId paInstanceNameId,
             const SFBInterfaceSpec &paInterfaceSpecPlug,
             bool paIsPlug);
    ~CAdapter() override;

    bool initialize() override;

    bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

    /*!\brief Returns if Adapter instance is a Plug
     */
    bool isPlug() const {
      return mIsPlug;
    }

    /*!\brief Returns if Adapter instance is a Socket
     */
    bool isSocket() const {
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
    bool isCompatible(const CAdapter &paPeer) const;

    const TForteInt16 *getEventInputWithIndices() const {
      return getFBInterfaceSpec().mEIWithIndexes;
    }

    const TForteInt16 *getEventOutputWithIndices() const {
      return getFBInterfaceSpec().mEOWithIndexes;
    }

    const TDataIOID *getEventInputWith() const {
      return getFBInterfaceSpec().mEIWith;
    }

    const TDataIOID *getEventOutputWith() const {
      return getFBInterfaceSpec().mEOWith;
    }

    CAdapter *getPeer() {
      return mPeer;
    }

    CAdapterConnection *getAdapterConnection() const {
      return mAdapterConn;
    }

  protected:
    void fillEventEntryList();

    std::vector<TEventID> mOutputEventIds;
    TForteUInt16 mParentAdapterListEventID;

    // TODO remove old lists when new adapter concept is implemented
    CDataConnection **mDIConns; //!< A list of data connections pointers storing for each data input the data
                                //!< connection. If the data input is not connected the pointer is nullptr.
    CGenDataConnection *mDOConns; //!< A list of data connections pointers storing for each data output the data
                                  //!< connection. If the data output is not connected the pointer is nullptr.

    CIEC_ANY **mDIs; //!< A list of pointers to the data inputs. This allows to implement a general getDataInput()
    CIEC_ANY **mDOs; //!< A list of pointers to the data outputs. This allows to implement a general

  private:
    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;
    void setupEventEntryList();

    const bool mIsPlug;
    CAdapter *mPeer;
    CIEC_ANY **mLocalDIs;
    CAdapterConnection *mAdapterConn;
    CFunctionBlock *mParentFB;
};
