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
#include <string_view>
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
       * \return The result of opening the channel; channel implementations may provide
       * additional results.
       * \retval Ok The channel was opened successfully or was already open and left unchanged
       * \retval InvalidId The channel identifier, address, configuration, or descriptors are invalid
       * \retval Overflow A required size or resource capacity cannot be accommodated
       * \retval DataTypeError The configured data type or required observer data is incompatible or unavailable
       * \retval Unknown The operation failed for an unspecified reason
       * \retval NoSocket A required socket or communication endpoint is unavailable
       * \retval InProgress The operation could not be completed immediately and requires further progress or retry
       * \retval NameNotResolved The configured host or service name could not be resolved
       * \retval NameResolutionTemporaryFailure Name or service resolution failed temporarily
       * \retval AddressInUse The requested address or endpoint is already in use
       * \retval AddressNotAvailable The requested address or endpoint is unavailable
       * \retval ConnectionRefused The remote endpoint refused the connection
       * \retval NetworkUnreachable The destination network cannot be reached
       * \retval HostUnreachable The destination host cannot be reached
       * \retval NetworkDown The local or connected network is unavailable
       * \retval TimedOut The operation did not complete before its time limit
       * \retval WouldBlock The operation cannot proceed under the current conditions; retry when they change
       * \retval Interrupted The operation was interrupted before completion
       * \retval PermissionDenied The operation is not permitted
       * \note If the channel is already open, the config string and descriptors are ignored. While opening is currently
       * in progress, any changes in either config string or descriptors may be ignored.
       */
      virtual ComResult open(std::string_view paConfigString, std::span<const ComChannelDescriptor> paDescriptors) = 0;

      /**
       * \brief Send data via the channel
       * \param paData The data to send
       * \return The result of sending the data; channel implementations may provide
       * additional results.
       * \retval Ok The data was sent successfully
       * \retval NoSocket No usable socket, channel, or peer is available for sending
       * \retval SendFailed The data could not be sent successfully
       * \retval DataTypeError The data has an unsupported type or shape
       * \retval WouldBlock The send cannot proceed under the current conditions; retry when they change
       * \retval ConnectionRefused The remote endpoint refused the communication
       * \retval ConnectionReset The connection was reset by the remote endpoint
       * \retval ConnectionAborted The connection was aborted
       * \retval ConnectionClosed The connection's sending direction is closed
       * \retval NetworkUnreachable The destination network cannot be reached
       * \retval HostUnreachable The destination host cannot be reached
       * \retval NetworkDown The local or connected network is unavailable
       * \retval TimedOut The operation did not complete before its time limit
       * \retval Interrupted The operation was interrupted before completion
       * \retval PermissionDenied The operation is not permitted
       * \retval NotConnected The channel is not connected to a peer
       * \retval MessageTooLarge The data exceeds the supported message size
       */
      virtual ComResult send(T paData) = 0;

      /**
       * \brief Poll the channel for data to receive
       * \return The result of polling the channel; channel implementations may provide
       * additional results.
       * \retval Async Data was delivered through the observer or is handled asynchronously by the channel
       * \retval More Additional data is required before the receive condition can be satisfied
       * \retval WouldBlock The receive cannot proceed under the current conditions; retry when they change
       * \retval NoSocket No usable socket, channel, or peer is available for receiving
       * \retval RecvFailed Data could not be received successfully
       * \retval Unknown The operation failed for an unspecified reason
       * \retval InProgress The operation could not be completed immediately and requires further progress or retry
       * \retval ConnectionRefused The remote endpoint refused the communication
       * \retval ConnectionReset The connection was reset by the remote endpoint
       * \retval ConnectionAborted The connection was aborted
       * \retval NetworkUnreachable The destination network cannot be reached
       * \retval HostUnreachable The destination host cannot be reached
       * \retval NetworkDown The local or connected network is unavailable
       * \retval Interrupted The operation was interrupted before completion
       * \retval PermissionDenied The operation is not permitted
       * \retval MessageTooLarge The received data exceeds the supported message size
       */
      virtual ComResult poll() = 0;

      /**
       * \brief Close the channel
       * \return The result of closing the channel; channel implementations may provide
       * additional results.
       * \retval Ok The channel was closed successfully
       * \retval NoSocket No usable socket or channel is available to close
       * \retval Unknown The operation failed for an unspecified reason
       * \retval Interrupted The operation was interrupted before completion
       * \retval InProgress The operation could not be completed immediately and requires further progress or retry
       */
      virtual ComResult close() = 0;

      /**
       * \brief Check whether the most recent open operation completed successfully
       * \return true until the channel is logically closed
       */
      [[nodiscard]] virtual bool isOpen() const = 0;

      /**
       * \brief Check whether the descriptors can be handled by the current channel instance
       * \param paDescriptors The descriptors to check against
       * \return true if the channel supports the descriptors
       */
      [[nodiscard]] virtual bool supports(std::span<const ComChannelDescriptor> paDescriptors) const = 0;

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
       * \return The result of setting the minimum; channel implementations may provide
       * additional results.
       * \retval Ok The minimum receive size was set successfully
       * \retval Overflow The requested size or required capacity cannot be accommodated
       * \retval DataTypeError The requested size is unsupported by the channel's data type
       */
      virtual ComResult setMinReceiveSize(const std::size_t paSize) {
        mMinReceiveSize = paSize;
        return ComResult::Ok;
      };

      /**
       * \brief Set the maximum size of received data
       * \param paSize The maximum size (0 to disable)
       * \return The result of setting the maximum; channel implementations may provide
       * additional results.
       * \retval Ok The maximum receive size was set successfully
       * \retval Overflow The requested size or required capacity cannot be accommodated
       * \retval DataTypeError The requested size is unsupported by the channel's data type
       */
      virtual ComResult setMaxReceiveSize(const std::size_t paSize) {
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

      [[nodiscard]] static bool supportsChannel(const StringId paChannel,
                                                const std::span<const ComChannelDescriptor> paDescriptors) {
        return !paDescriptors.empty() && paDescriptors.front().mChannel == paChannel;
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
