/*******************************************************************************
 * Copyright (c) 2025 Jose Cabral
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral- initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <functional>

#include "../../../core/src/trace/internal/EventMessage.h"
#include "forte/stringid.h"
#include "forte/event.h"

namespace forte {
  class CResource;
  class CFakeEventExecutionThread;

  namespace iec61499::system {
    /**
     * @brief Takes external control of the ecet of a resource and reproduce all the events based on a list of external
     * events. On destructoin, the ecet is released and let run freely.
     *
     */
    class CResourceReplayer {
      public:
        /**
         * @brief Constructor
         *
         * @param paResource the resource being replayed
         */
        CResourceReplayer(CResource &paResource, std::vector<trace::EventMessage> paExternalEvents);

        ~CResourceReplayer();

        CResourceReplayer(const CResourceReplayer &) = delete;
        CResourceReplayer &operator=(const CResourceReplayer &) = delete;

        /**
         * @brief Move constructor. Transfers ownership and keep the ecet in external control mode
         *
         * @param paOther object to construct from
         */
        CResourceReplayer(CResourceReplayer &&paOther);
        CResourceReplayer &operator=(CResourceReplayer &&paOther) = delete;

        /**
         * @brief Executes the algorithm tha generates the full set of events of the resource
         *
         * @param paExternalEvents list of output events of Service Function Blocks
         * @return list of full events of the resource
         */
        std::vector<trace::EventMessage> reproduceAll();

        /**
         * @brief Reproduce the next event
         *
         * @return the event that was executed, std::nullopt if nothing was executed
         */
        std::optional<TEventEntry> reproduceNextEvent();

        /**
         * @brief Get all generated events in the resource
         *
         * @return all generated events
         */
        std::vector<trace::EventMessage> getGeneratedEvents();

      private:
        CResource &mResource;

        CFakeEventExecutionThread &mEcet;

        size_t mStepperIndex{0};

        const std::vector<trace::EventMessage> mExternalEvents;

        /// member that releases control of the ecet on destruction of the last object which was moved into
        std::function<void(void)> mReleaseEcet;
    };
  } // namespace iec61499::system
} // namespace forte
