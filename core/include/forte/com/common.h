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
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

#pragma once

#include <string_view>

#include "forte/stringid.h"

#include <vector>

namespace forte::com {
  enum class ComResult {
    /**
     * \brief Success
     *
     * The operation completed successfully.
     */
    Ok = 0,
    /**
     * \brief Unknown error
     *
     * An unknown error occurred.
     */
    Unknown = 1,
    /**
     * \brief Invalid ID
     *
     * The communication ID is invalid.
     */
    InvalidId = 2,
    /**
     * \brief Communication object terminated
     *
     * The communication object has been terminated.
     */
    Terminated = 3,
    /**
     * \brief Invalid object
     *
     * The communication object is invalid.
     */
    InvalidObject = 4,
    /**
     * \brief Data type error
     *
     * The data type is invalid.
     */
    DataTypeError = 5,
    /**
     * \brief Inhibited
     *
     * The operation is inhibited.
     */
    Inhibited = 6,
    /**
     * \brief No socket
     *
     * There is no open socket.
     */
    NoSocket = 7,
    /**
     * \brief Send failed
     *
     * The send operation failed.
     */
    SendFailed = 8,
    /**
     * \brief Recv failed
     *
     * The receive operation failed.
     */
    RecvFailed = 9,
    /**
     * \brief Asynchronous operation
     *
     * The operation is performed asynchronously.
     */
    Async = 10,
    /**
     * \brief More data is expected
     *
     * The operation should be attempted again when more data is available.
     */
    More = 11,
    /**
     * \brief Not enough resources
     *
     * There are not enough resources.
     */
    Overflow = 12,
    /**
     * \brief Address in use
     *
     * The requested address is already in use.
     */
    AddressInUse = 13,
    /**
     * \brief Address not available
     *
     * The requested address is not available.
     */
    AddressNotAvailable = 14,
    /**
     * \brief Connection refused
     *
     * The remote endpoint refused the connection.
     */
    ConnectionRefused = 15,
    /**
     * \brief Connection reset
     *
     * The connection was reset by the remote endpoint.
     */
    ConnectionReset = 16,
    /**
     * \brief Connection aborted
     *
     * The connection was aborted.
     */
    ConnectionAborted = 17,
    /**
     * \brief Connection closed
     *
     * The connection's send direction is closed.
     */
    ConnectionClosed = 18,
    /**
     * \brief Network unreachable
     *
     * The network is unreachable.
     */
    NetworkUnreachable = 19,
    /**
     * \brief Host unreachable
     *
     * The host is unreachable.
     */
    HostUnreachable = 20,
    /**
     * \brief Network down
     *
     * The local network is down.
     */
    NetworkDown = 21,
    /**
     * \brief Timed out
     *
     * The operation timed out.
     */
    TimedOut = 22,
    /**
     * \brief Would block
     *
     * The operation would block.
     */
    WouldBlock = 23,
    /**
     * \brief Interrupted
     *
     * The operation was interrupted.
     */
    Interrupted = 24,
    /**
     * \brief Permission denied
     *
     * Permission was denied for the operation.
     */
    PermissionDenied = 25,
    /**
     * \brief Not connected
     *
     * The socket is not connected.
     */
    NotConnected = 26,
    /**
     * \brief Message too large
     *
     * The message is too large for the transport.
     */
    MessageTooLarge = 27,
    /**
     * \brief Name not resolved
     *
     * The name could not be resolved.
     */
    NameNotResolved = 28,
    /**
     * \brief Temporary name resolution failure
     *
     * Name resolution failed temporarily.
     */
    NameResolutionTemporaryFailure = 29,
    /**
     * \brief Operation in progress
     *
     * The operation could not be completed immediately, it is in progress asynchronously and should be retried later.
     */
    InProgress = 30,
  };

  /**
   * \brief A value and its communication result
   */
  template<typename T>
  struct ComResultValue {
      ComResult mResult{ComResult::Ok};
      T mValue{};

      [[nodiscard]] explicit operator bool() const noexcept {
        return mResult == ComResult::Ok;
      }
  };

  struct ComChannelDescriptor {
      StringId mChannel{};
      std::string_view mConfigString;
  };

  /**
   * \brief Get a string for the result
   * \param paResult The result
   * \return The string
   */
  std::string_view getComResultString(ComResult paResult);

  std::vector<ComChannelDescriptor> parseComId(std::string_view paId);
} // namespace forte::com
