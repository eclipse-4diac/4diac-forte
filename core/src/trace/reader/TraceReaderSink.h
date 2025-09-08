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

#include <string>
#include <vector>

#include <babeltrace2/babeltrace.h>

#include "../internal/EventMessage.h"

namespace forte::trace::reader {
  /**
   * Used with the babeltrace2 library to read event messages from it and transform them
   * into EventMessage objects
   */
  class EventsReader {
    public:
      typedef std::vector<EventMessage> MessageStorage;

      EventsReader(bt_self_component_sink *self_component_sink,
                   const bt_value *params,
                   MessageStorage &initialize_method_data);

      bt_component_class_sink_graph_is_configured_method_status graphIsConfigured();

      bt_component_class_sink_consume_method_status consume();

    private:
      bt_self_component_sink *mSelfComponentSink;

      MessageStorage &mOutput;

      bt_message_iterator *mMessageIterator{nullptr};
  };
} // namespace forte::trace::reader
