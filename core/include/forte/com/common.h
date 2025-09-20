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
