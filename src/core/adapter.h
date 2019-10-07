/*******************************************************************************
 * Copyright (c) 2008 - 2015 ACIN, fortiss GmbH, 2018 TU Vienna/ACIN
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
 *******************************************************************************/
#ifndef _ADAPTER_H_
#define _ADAPTER_H_

#include "funcbloc.h"

class CAdapterConnection;

#define ADAPTER_CTOR(fbclass) \
 fbclass(CStringDictionary::TStringId pa_anAdapterInstanceName, CResource *pa_poSrcRes, bool pa_bIsPlug) : \
 CAdapter( pa_poSrcRes, &scm_stFBInterfaceSpecSocket, pa_anAdapterInstanceName, &scm_stFBInterfaceSpecPlug, pa_bIsPlug, m_anFBConnData, m_anFBVarsData)

#define ADAPTER_CTOR_WITH_BASE_CLASS(fbclass, fbBaseClass) \
 fbclass(CStringDictionary::TStringId pa_anAdapterInstanceName, CResource *pa_poSrcRes, bool pa_bIsPlug) : \
 fbBaseClass( pa_poSrcRes, &scm_stFBInterfaceSpecSocket, pa_anAdapterInstanceName, &scm_stFBInterfaceSpecPlug, pa_bIsPlug, m_anFBConnData, m_anFBVarsData)

#define FORTE_ADAPTER_DATA_ARRAY(a_nNumEIs, a_nNumEOs, a_nNumDIs, a_nNumDOs, a_nNumAdapters) \
  union{ \
    TForteByte m_anFBConnData[genAdapterFBConnDataSizeTemplate<a_nNumEIs, a_nNumEOs, a_nNumDIs, a_nNumDOs>::value]; \
  };\
  union{ \
    TForteByte m_anFBVarsData[genFBVarsDataSizeTemplate<a_nNumDIs, a_nNumDOs, a_nNumAdapters>::value]; \
  };

/*!\ingroup CORE\brief Class for handling adapters.
 *
 */
class CAdapter : public CFunctionBlock{
  public:
    /*!\brief The main constructor for an adapter instance.
     */
    //TODO: think on interface
    CAdapter(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpecSocket, const CStringDictionary::TStringId pa_nInstanceNameId, const SFBInterfaceSpec *pa_pstInterfaceSpecPlug, bool pa_bIsPlug, TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData);
    virtual ~CAdapter();

    /*!\brief Returns if Adapter instance is a Plug
     */
    bool isPlug() const{
      return m_bIsPlug;
    }

    /*!\brief Returns if Adapter instance is a Socket
     */
    bool isSocket() const{
      return !m_bIsPlug;
    }

    virtual void setParentFB(CFunctionBlock *pa_poParentFB, TForteUInt8 pa_nParentAdapterlistID);

    /*! \brief Connects adapter to peer adapter
     *
     *   With this command the adapter is connected to a complementary adapter instance (peer).
     *   The data output pointers are "redirected" to the data inputs of the peer.
     *   \param pa_poPeer  Pointer to the peer to connect to.
     *   \param pa_poAdConn Pointer to the connecting adapter connection.
     *   \return success of establishment of connection
     */
    bool connect(CAdapter *pa_poPeer, CAdapterConnection *pa_poAdConn);

    /*! \brief Disconnects adapter from peer adapter
     *
     *   With this command the adapter is disconnected from a complementary adapter instance (peer).
     *   The redirection of data output pointers is removed.
     *   \param pa_poAdConn Pointer to the connecting adapter connection.
     *   \return success of disconnection
     */
    virtual bool disconnect(CAdapterConnection *pa_poAdConn = 0);

    /*! \brief Returns the compatibility of the adapter to another adapter
     *
     *   This method will evaluate the compatibility of this adapter to another one.
     *   Only instances of compatible adapter types may be interconnected.
     *   \param pa_poPeer Pointer to a potential peer, whose compatibility has to be checked.
     *   \return compatibility status
     */
    bool isCompatible(CAdapter *pa_poPeer) const;

    /*! provides access to the interface spec used for the specific adatpter instnace (i.e., plug or socket)
     *  Is required mainly by the anyadapter
     */
    const SFBInterfaceSpec* getAdapterInterfaceSpec() const{
      return m_pstInterfaceSpec;
    }

    const TForteInt16 *getEventInputWithIndices() const{
      return getAdapterInterfaceSpec()->m_anEIWithIndexes;
    }

    const TForteInt16 *getEventOutputWithIndices() const{
      return getAdapterInterfaceSpec()->m_anEOWithIndexes;
    }

    const TDataIOID *getEventInputWith() const{
      return getAdapterInterfaceSpec()->m_anEIWith;
    }

    const TDataIOID *getEventOutputWith() const{
      return getAdapterInterfaceSpec()->m_anEOWith;
    }

    CAdapter *getPeer(){
      return m_poPeer;
    }

    CAdapterConnection *getAdapterConnection() const{
      return m_poAdapterConn;
    }

    static size_t genAdapterFBConnDataSize(unsigned int pa_nNumEIs, unsigned int pa_nNumEOs, unsigned int pa_nNumDIs, unsigned int pa_nNumDOs){
      return genFBConnDataSize(((pa_nNumEIs < pa_nNumEOs) ? pa_nNumEOs : pa_nNumEIs), ((pa_nNumDIs < pa_nNumDOs) ? pa_nNumDOs : pa_nNumDIs), ((pa_nNumDIs < pa_nNumDOs) ? pa_nNumDOs : pa_nNumDIs)); //for outputs data connections are stored in this array, as plugs and sockets have invoerted size wee need to use the max here. unfortunately not very efficient. should be reconsidered.
    }

  protected:
    template<unsigned int ta_nNumEIs, unsigned int ta_nNumEOs, unsigned int ta_nNumDIs, unsigned int ta_nNumDOs>
    struct genAdapterFBConnDataSizeTemplate{
        enum {
          value = genFBConnDataSizeTemplate<((ta_nNumEIs < ta_nNumEOs) ? ta_nNumEOs : ta_nNumEIs), ((ta_nNumDIs < ta_nNumDOs) ? ta_nNumDOs : ta_nNumDIs), ((ta_nNumDIs < ta_nNumDOs) ? ta_nNumDOs : ta_nNumDIs)>::value
        };
    };

    /**!
     *  @brief fills the event entry list of an adapter
     *
     *  @param paParentFB The FB which shall be entered into the event entry list
     */
    void fillEventEntryList(CFunctionBlock* paParentFB);

    TForteUInt16 m_nParentAdapterListEventID;

  private:
    virtual void executeEvent(int pa_nEIID);
    void setupEventEntryList();

    const bool m_bIsPlug;
    CAdapter *m_poPeer;
    CIEC_ANY *m_aoLocalDIs;
    CAdapterConnection *m_poAdapterConn;
    SEventEntry *m_astEventEntry; //! the event entry list to start the event chain
};

#endif /*_ADAPTER_H_*/
