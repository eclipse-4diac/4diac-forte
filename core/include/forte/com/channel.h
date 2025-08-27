/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Jobst - initial implementation
 *******************************************************************************/

#pragma once

#include <span>
#include <string>
#include <utility>

#include "forte/com/common.h"
#include "forte/com/observer.h"

namespace forte::com {

  template<typename T>
  class ComChannel {
    public:
      /**
       * \brief Open a channel with the given address and descriptors
       * \param paConfigString The configuration string for the channel
       * \param paDescriptors Communication channel descriptors for configuration
       * \return The result of opening the channel
       * \retval Ok The channel was opened successfully
       * \retval InvalidId The address or descriptors are invalid
       * \retval Overflow There are not enough resources
       */
      virtual ComResult open(std::string_view paConfigString, std::span<ComChannelDescriptor> paDescriptors) = 0;

      /**
       * \brief Send data via the channel
       * \param paData The data to send
       * \return The result of sending the data
       * \retval Ok The data was sent successfully
       * \retval Retry Retry sending the data
       */
      virtual ComResult send(T paData) = 0;

      /**
       * \brief Poll the channel for data to receive
       * \return The result of polling the channel
       */
      virtual ComResult poll() = 0;

      /**
       * \brief Close the channel
       * \return The result of closing the channel
       */
      virtual ComResult close() = 0;

      /**
       * \brief Get the minimum size of received data
       * \return The current minimum size of received data
       * \retval 0 indicates no minimum
       */
      [[nodiscard]] std::size_t getMinReceiveSize() const {
        return mMinReceiveSize;
      }

      /**
       * \brief Get the maximum size of received data
       * \return The current maximum size of received data
       * \retval 0 indicates receive is disabled
       */
      [[nodiscard]] std::size_t getMaxReceiveSize() const {
        return mMaxReceiveSize;
      }

      /**
       * \brief Set the minimum size of received data
       * \param paSize The minimum size
       * \return The result of setting the minimum
       * \retval Ok The size was set successfully
       * \retval Overflow There are not enough resources
       */
      virtual ComResult setMinReceiveSize(std::size_t paSize) {
        mMinReceiveSize = paSize;
        return ComResult::Ok;
      };

      /**
       * \brief Set the maximum size of received data
       * \param paSize The maximum size (0 to disable)
       * \return The result of setting the maximum
       * \retval Ok The size was set successfully
       * \retval Overflow There are not enough resources
       */
      virtual ComResult setMaxReceiveSize(std::size_t paSize) {
        mMaxReceiveSize = paSize;
        return ComResult::Ok;
      };

      explicit ComChannel(ComObserver<T> &paObserver) : mObserver(paObserver), mMinReceiveSize(0), mMaxReceiveSize(0) {
      }

      virtual ~ComChannel() = default;

    protected:
      ComObserver<T> &getObserver() {
        return mObserver;
      }

    private:
      ComObserver<T> &mObserver;
      std::size_t mMinReceiveSize;
      std::size_t mMaxReceiveSize;
  };

  template<typename T, typename U>
  concept ComChannelImpl = requires(T t) {
    { T(std::declval<ComObserver<U> &>()) };
    requires std::derived_from<T, ComChannel<U>>;
  };
} // namespace forte::com
