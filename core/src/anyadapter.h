/*******************************************************************************
 * Copyright (c) 2013, 2025 fortiss GmbH, TU Vienna/ACIN,
 *                          Johannes Kepler Universty Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians
 *               - fixes event chain initialisation, adds typifyAnyAdapter
 *   Alois Zoitl - re-implementation for new communication infrastructure
 *******************************************************************************/
#pragma once

#include "forte/adapterconn.h"

namespace forte {

  class CAnyAdapterPin : public IAdapterPin {
    public:
      CAnyAdapterPin(StringId paInstanceNameId);
      ~CAnyAdapterPin();

    protected:
      void createConfiguredAdapter(CAdapter *paPeer,
                                   CFunctionBlock &paParent,
                                   bool paIsPlug,
                                   TForteUInt8 paParentAdapterlistID);
      void removeConfiguredAdapter();

      StringId mInstanceNameId;
      std::unique_ptr<CAdapter> mConfiguredAdapter;
  };

  class CAnyPlugPin final : public CAnyAdapterPin, public IPlugPin {

    public:
      CAnyPlugPin(StringId paInstanceNameId, CFunctionBlock &paParentFB, TForteUInt8 paParentAdapterlistID);

      CAdapter *getAdapterBlock() override;

      StringId getAdapterTypeId() const override;

      bool isCompatible(IAdapterPin &paPeer) override;

      CAdapterConnection &getAdapterCon() override;

      void setPeer(CAdapter *paPeer) override;

    private:
      void removeConfiguredAdapter();

      CAdapterConnection mAdapterCon;
  };

  class CAnySocketPin final : public CAnyAdapterPin, public ISocketPin {

    public:
      CAnySocketPin(StringId paInstanceNameId, CFunctionBlock &paParentFB, TForteUInt8 paParentAdapterlistID);

      CAdapter *getAdapterBlock() override;

      StringId getAdapterTypeId() const override;

      bool isCompatible(IAdapterPin &paPeer) override;

      bool connect(CAdapterConnection &paConn) override;

      void disconnect() override;

      CAdapterConnection *getAdapterCon() override {
        return mAdapterCon;
      }

    private:
      CAdapterConnection *mAdapterCon;
      CFunctionBlock &mParentFB;
      const TForteUInt8 mParentAdapterlistID;
  };

} // namespace forte
