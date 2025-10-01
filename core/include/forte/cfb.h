/*******************************************************************************
 * Copyright (c) 2005, 2025 Profactor GmbH, ACIN, fortiss GmbH,
 *                          Johannes Kepler University Linz
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Gunnar Grabmaier, Gerhard Ebenhofer,
 *    Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *    Alois Zoitl - added support for adapter connections in CFBs
 *    Martin Jobst - add smart pointer for internal FBs
 *******************************************************************************/

#pragma once

#include "forte/funcbloc.h"

namespace forte {
  struct SCFB_FBConnectionData {
      StringId mSrcFBNameId;
      StringId mSrcId;
      StringId mDstFBNameId;
      StringId mDstId;
  };

  struct SCFB_FBNData {
      std::span<const SCFB_FBConnectionData> mEventConnections;
      std::span<const SCFB_FBConnectionData> mDataConnections;
      std::span<const SCFB_FBConnectionData> mAdapterConnections;
  };

  /*!\ingroup CORE
   * \brief Class for handling firmware composite function blocks.
   */
  class CCompositeFB : public CFunctionBlock {

    public:
      ~CCompositeFB() override = default;

      bool initialize() override;

      /*!\brief Connects specific data input of a CFB with a specific data connection.
       *
       * \param paDINameId    StringId of input name.
       * \param paDataCon     Pointer to the data connection the CFB should be connected to.
       * \return TRUE on success, FALSE if data output not exists or output is already connected.
       */
      bool connectDI(TPortId paDIPortId, CDataConnection *paDataCon) override;

      bool configureGenericDI(TPortId paDIPortId, const CIEC_ANY &paRefValue) override;
      bool configureGenericDIO(TPortId paDIOPortId, const CIEC_ANY &paRefValue) override;
      bool configureGenericDO(TPortId paDOPortId, const CIEC_ANY &paRefValue) override;

      EMGMResponse changeExecutionState(EMGMCommandType paCommand) override;

    protected:
      /*!\brief The main constructor for a composite function block.
       *
       * The interface of the constructor has been extended from the basic FB's interface with the following values:
       * \param paContainer      reference to the container of this CFB
       * \param paInterfaceSpec  pointer to interface specification
       * \param paInstanceNameId StringId of instance name
       * \param paFBNData        const reference to description of internal structure of FB (FBs, Connections, ...)
       */
      CCompositeFB(CFBContainer &paContainer,
                   const SFBInterfaceSpec &paInterfaceSpec,
                   StringId paInstanceNameId,
                   const SCFB_FBNData &paFBNData);
      void setInitialValues() override = 0;

      const SCFB_FBNData &getFBNData() const {
        return cmFBNData;
      }

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) final;

    private:
      virtual bool createInternalFBs() {
        return true;
      }

      void createEventConnections();
      void prepareIf2InEventCons();

      void establishConnection(CConnection *paCon, CFunctionBlock &paDstFb, std::span<const StringId> paDstNameId);
      void createDataConnections();
      CDataConnection *getDataConn(CFunctionBlock *paSrcFB, StringId paSrcNameId);
      void createAdapterConnections();
      virtual void setFBNetworkInitialValues();

      //! Acquire the function block for a given function block instance name id this may be a contained fb, an adapter,
      //! or the composite itself.
      CFunctionBlock *getFunctionBlock(StringId paFBNameId);

      virtual CDataConnection *getIf2InConUnchecked(TPortId) = 0;
      virtual CInOutDataConnection *getDIOOutConInternalUnchecked(TPortId) {
        return nullptr;
      }

      const SCFB_FBNData &cmFBNData;

      //! The connections to be used in the execute event for triggering the internal FBs
      std::vector<std::unique_ptr<CEventConnection>> mInterface2InternalEventCons;

#ifdef FORTE_FMU
      friend class fmuInstance;
#endif // FORTE_FMU
  };
} // namespace forte
