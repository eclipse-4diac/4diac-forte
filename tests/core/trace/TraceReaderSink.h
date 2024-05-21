#ifndef _FORTE_TESTS_CORE_TRACE_TRACE_READER_SINK_H_
#define _FORTE_TESTS_CORE_TRACE_TRACE_READER_SINK_H_


#include <vector>
#include <string>

#include <babeltrace2/babeltrace.h>

#include "EventMessage.h"

namespace forte {
  class EventsReader {
    public:

    typedef std::vector<EventMessage> MessageStorage;

    EventsReader(bt_self_component_sink *self_component_sink,
        const bt_value *params, 
        MessageStorage& initialize_method_data);

    bt_component_class_sink_graph_is_configured_method_status graphIsConfigured();

    bt_component_class_sink_consume_method_status consume();

    private:
    bt_self_component_sink *mSelfComponentSink;

    MessageStorage& mOutput;

    bt_message_iterator *mMessageIterator{nullptr};
  };  
}

#endif // _FORTE_TESTS_CORE_TRACE_TRACE_READER_SINK_H_