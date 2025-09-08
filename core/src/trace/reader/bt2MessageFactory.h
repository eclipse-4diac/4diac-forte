/*******************************************************************************
 * Copyright (c) 2024 Jose Cabral
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

#include "../internal/EventMessage.h"

class bt_message;

namespace forte::trace::reader::Bt2MessageFactory {
  /**
   * @brief babeltrace2 message factory
   *
   * Creates an EventMessage from the babeltrace type. If any errors occur, the program aborts
   *
   * @param paMessage the message in babaeltrace format
   * @return EventMessage the created event message
   */
  EventMessage createMessage(const bt_message *paMessage);
}; // namespace forte::trace::reader::Bt2MessageFactory
