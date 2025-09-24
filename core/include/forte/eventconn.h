/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl,
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#pragma once

#include <vector>
#include "forte/conn.h"

namespace forte {
  class CEventChainExecutionThread;

  /*!\ingroup CORE \brief Class for handling an event connection.
   *
   */
  class CEventConnection : public CConnection {
    public:
      CEventConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId);

      ~CEventConnection() override;

      EMGMResponse connect(CFunctionBlock &paDstFB, std::span<const StringId> paDstPortNameId) override;

      EMGMResponse connectToCFBInterface(CFunctionBlock &paDstFB, std::span<const StringId> paDstPortNameId) override;

      EMGMResponse disconnect(CFunctionBlock &paDstFB, std::span<const StringId> paDstPortNameId) override;

      void getSourcePortName(TNameIdentifier &paResult) const override;

      /*! \brief Triggers the event connection and all destinations are notified.
       *
       *
       *  \param paExecEnv Pointer to the execution environment the event is sent in.
       */
      void triggerEvent(CEventChainExecutionThread *paExecEnv) const;

      /*! \brief Check if there are destinations added to this connection
       *
       * \return true if there are destinations
       */
      bool isConnected() const {
        return !mDestinationIds.empty();
      }

      /*! \brief Get list of destinations of the connection
       */
      const std::vector<CConnectionPoint> &getDestinationList() const {
        return mDestinationIds;
      }

    protected:
      EMGMResponse addDestination(const CConnectionPoint &paDestPoint);

      EMGMResponse removeDestination(const CConnectionPoint &paDestPoint);

    private:
      /*!\brief a list of destinations the connection is connected to.
       *
       * By storing a list of destinations an implicit support for fan-out is given.
       * The destination is represented as string id the same way as the sourceId
       */
      std::vector<CConnectionPoint> mDestinationIds;
  };
} // namespace forte
