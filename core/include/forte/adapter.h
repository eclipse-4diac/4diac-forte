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

#include "forte/adapterconn.h"
#include "forte/datatype.h"
#include "forte/funcbloc.h"

namespace forte {

  class CAdapterConnection;

  constexpr TForteUInt8 cgCFBParentAdapterlistIDMarker = static_cast<TForteUInt8>(-1);

  /*!\ingroup CORE\brief Class for handling adapters.
   *
   */
  class CAdapter : public CFunctionBlock {
    public:
      ~CAdapter() override = default;

      void setPeer(CAdapter *paPeer) {
        mPeer = paPeer;
      }

      CAdapter *getPeer() {
        return mPeer;
      }

    protected:
      CAdapter(CFBContainer &paContainer,
               const SFBInterfaceSpec &paInterfaceSpec,
               const StringId paInstanceNameId,
               TForteUInt8 paParentAdapterlistID);

      TForteUInt16 getParentAdapterListEventID() const {
        return mParentAdapterListEventID;
      }

    private:
      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      TForteUInt16 mParentAdapterListEventID;

      CAdapter *mPeer;
  };

  template<typename T>
  class CPlugPin final : public CInternalFB<T>, public IPlugPin {
      static_assert(std::is_base_of_v<CAdapter, T>, "T must be a CAdapter");

    public:
      CPlugPin(StringId paInstanceNameId, CFunctionBlock &paParentFB, TForteUInt8 paParentAdapterlistID) :
          CInternalFB<T>(paInstanceNameId, paParentFB, paParentAdapterlistID),
          mAdapterCon(paParentFB, paParentAdapterlistID, *this) {
      }

      T *getAdapterBlock() override {
        return CInternalFB<T>::get();
      }

      const T *getAdapterBlock() const {
        return const_cast<CPlugPin<T> *>(this)->getAdapterBlock();
      }

      StringId getAdapterTypeId() const override {
        return getAdapterBlock()->getFBTypeId();
      }

      bool isCompatible(IAdapterPin &paPeer) override {
        return getAdapterTypeId() == paPeer.getAdapterTypeId();
      }

      CAdapterConnection &getAdapterCon() override {
        return mAdapterCon;
      }

      void setPeer(CAdapter *paPeer) override {
        getAdapterBlock()->setPeer(paPeer);
      }

    private:
      CAdapterConnection mAdapterCon;
  };

  template<typename T>
  class CSocketPin final : public CInternalFB<T>, public ISocketPin {
      static_assert(std::is_base_of_v<CAdapter, T>, "T must be a CAdapter");

    public:
      CSocketPin(StringId paInstanceNameId, CFunctionBlock &paParentFB, TForteUInt8 paParentAdapterlistID) :
          CInternalFB<T>(paInstanceNameId, paParentFB, paParentAdapterlistID),
          mAdapterCon(nullptr) {
      }

      T *getAdapterBlock() override {
        return CInternalFB<T>::get();
      }

      const T *getAdapterBlock() const {
        return const_cast<CSocketPin<T> *>(this)->getAdapterBlock();
      }

      StringId getAdapterTypeId() const override {
        return getAdapterBlock()->getFBTypeId();
      }

      bool isCompatible(IAdapterPin &paPeer) override {
        return getAdapterTypeId() == paPeer.getAdapterTypeId();
      }

      bool connect(CAdapterConnection &paConn) override {
        paConn.getPlug().setPeer(getAdapterBlock());
        mAdapterCon = &paConn;
        getAdapterBlock()->setPeer(paConn.getPlug().getAdapterBlock());
        return true;
      }

      void disconnect() override {
        mAdapterCon = nullptr;
        getAdapterBlock()->setPeer(nullptr);
      }

      CAdapterConnection *getAdapterCon() override {
        return mAdapterCon;
      }

    private:
      CAdapterConnection *mAdapterCon;
  };

} // namespace forte
