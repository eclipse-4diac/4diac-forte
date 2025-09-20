/*******************************************************************************
 * Copyright (c) 2008, 2025 ACIN, fortiss GmbH, 2018 TU Vienna/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Ingo Hegny, Alois Zoitl, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - adds typifyAnyAdapter
 *******************************************************************************/
#pragma once

#include "forte/conn.h"

namespace forte {

  class CAdapter;
  class IPlugPin;
  class ISocketPin;

  /*! \ingroup CORE\brief Class for handling a adapter connection.
   */
  class CAdapterConnection : public CConnection {
    public:
      CAdapterConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId, IPlugPin &paPlug);
      ~CAdapterConnection() override;

      EMGMResponse connect(CFunctionBlock &paDstFB, std::span<const StringId> paDstPortNameId) override;

      EMGMResponse connectToCFBInterface(CFunctionBlock &paDstFB, std::span<const StringId> paDstPortNameId) override;

      EMGMResponse disconnect(CFunctionBlock &paDstFB, std::span<const StringId> paDstPortNameId) override;

      void getSourcePortName(TNameIdentifier &paResult) const override;

      IPlugPin &getPlug() {
        return mPlug;
      }

      const IPlugPin &getPlug() const {
        return mPlug;
      }

      ISocketPin *getSocket() {
        return mSocket;
      }

      const ISocketPin *getSocket() const {
        return mSocket;
      }

    private:
      void performDisconnect();

      IPlugPin &mPlug;

      ISocketPin *mSocket;
  };

  class IAdapterPin {
    public:
      virtual CAdapter *getAdapterBlock() = 0;
      virtual bool isCompatible(IAdapterPin &paPeer) = 0;
      virtual StringId getAdapterTypeId() const = 0;
  };

  class IPlugPin : public IAdapterPin {
    public:
      virtual CAdapterConnection &getAdapterCon() = 0;
      virtual void setPeer(CAdapter *paPeer) = 0;
  };

  class ISocketPin : public IAdapterPin {
    public:
      virtual bool connect(CAdapterConnection &paConn) = 0;
      virtual void disconnect() = 0;
      virtual CAdapterConnection *getAdapterCon() = 0;

      const CAdapterConnection *getAdapterCon() const {
        return const_cast<ISocketPin *>(this)->getAdapterCon();
      }
  };

} // namespace forte
