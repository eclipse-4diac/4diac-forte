
#include <babeltrace2/babeltrace.h>

#include <vector>
#include <string>

namespace forte {
  class EventsReader {
    public:

    typedef std::vector<std::string> MessageStorage;

    EventsReader(bt_self_component_sink *self_component_sink,
        const bt_value *params, 
        MessageStorage& initialize_method_data);

    bt_component_class_sink_graph_is_configured_method_status graphIsConfigured();

    bt_component_class_sink_consume_method_status consume();

    private:
    void print_message(const bt_message *message);

    bt_self_component_sink *mSelfComponentSink;

    std::vector<std::string>& mOutput;

    bt_message_iterator *mMessageIterator{nullptr};
  };  
}
