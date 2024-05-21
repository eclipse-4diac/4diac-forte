#include "EventMessage.h"
#include <array>
#include <iomanip>

// ******************** //
// EventMessage methods //
// ******************** //

EventMessage::EventMessage(const bt_message* message)  {
    
  // Borrow the event message's event and its class
  const bt_event *event =
    bt_message_event_borrow_event_const(message);
  const bt_event_class *event_class = bt_event_borrow_class_const(event);

  const bt_field *payload_field =
      bt_event_borrow_payload_field_const(event);

  mEventType = bt_event_class_get_name(event_class);
  mPayload = PayloadFactory::createPayload(mEventType, payload_field);

  if(mPayload == nullptr){
    std::abort();
  }

  if(BT_CLOCK_SNAPSHOT_GET_NS_FROM_ORIGIN_STATUS_OK != 
      bt_clock_snapshot_get_ns_from_origin(
        bt_message_event_borrow_default_clock_snapshot_const(message), 
        &mTimestamp)){
    std::abort();
  }	
}

std::string EventMessage::getPayloadString() const {
  return mEventType + (mPayload != nullptr ? ": " + mPayload->getString() : "");
}

std::string EventMessage::getTimestampString() const {
  auto nanoseconds = mTimestamp;
  auto milliseconds = mTimestamp / 1000000;
  
  auto seconds = milliseconds / 1000;
  auto minutes = seconds / 60;
  auto hours = minutes / 60;

  nanoseconds %= 1000000000;
  seconds %= 60;
  minutes %= 60;

  std::stringstream ss;
  ss << "[" << 
      std::setw(2) << std::setfill('0') << hours + 1 << ":" <<
      std::setw(2) << std::setfill('0') << minutes << ":" <<
      std::setw(2) << std::setfill('0') << seconds << "." <<
      std::setw(6) << std::setfill('0') << nanoseconds <<  "]";

  return ss.str();
}

bool EventMessage::operator==(const EventMessage& other) const {
  return mEventType == other.mEventType  
    && (mPayload == nullptr) == (other.mPayload == nullptr)
    && (mPayload != nullptr ? *mPayload == *other.mPayload : true);
}

// *********************** //
// AbstractPayload Methods //
// *********************** //

AbstractPayload::AbstractPayload(const bt_field* field) {
  auto const typeNameField = bt_field_structure_borrow_member_field_by_name_const(field, "typeName");
  auto const instanceNameField = bt_field_structure_borrow_member_field_by_name_const(field, "instanceName");
  
  mTypeName = bt_field_string_get_value(typeNameField);
  instanceName = bt_field_string_get_value(instanceNameField);
}

std::string AbstractPayload::getString() const {
  return "{ typeName = \"" + mTypeName + "\", instanceName = \"" + instanceName + "\"" + specificPayloadString() + " }";
}


bool AbstractPayload::operator==(const AbstractPayload& other) const {
  return mTypeName == other.mTypeName && instanceName == other.instanceName && specificPayloadEqual(other);
}

// ************** //
// FBEventPayload //
// ************** //

FBEventPayload::FBEventPayload(const bt_field* field) : AbstractPayload(field){
  eventId = bt_field_integer_unsigned_get_value(
      bt_field_structure_borrow_member_field_by_name_const(field, "eventId"));
}

std::string FBEventPayload::specificPayloadString() const {
  return ", eventId = " + std::to_string(eventId);
}

bool FBEventPayload::specificPayloadEqual(const AbstractPayload& other) const {
  const auto& childInstance = dynamic_cast<const FBEventPayload&>(other); 
  return eventId == childInstance.eventId;
}

// ************* //
// FBDataPayload //
// ************* //

FBDataPayload::FBDataPayload(const bt_field* field) : AbstractPayload(field){
  dataId = bt_field_integer_unsigned_get_value(
      bt_field_structure_borrow_member_field_by_name_const(field, "dataId"));

  value = bt_field_string_get_value(
      bt_field_structure_borrow_member_field_by_name_const(field, "value"));
}

std::string FBDataPayload::specificPayloadString() const {
  auto convertedString = value;

  // add backslash to quoutes and double quotes
  for(auto toReplace : {"\"", "\'"}){
    for(auto pos = convertedString.find(toReplace); pos != std::string::npos; pos = convertedString.find(toReplace, pos + 2)){
      convertedString.insert(pos, "\\");
    }
  }
  return ", dataId = " + std::to_string(dataId) + ", value = \"" + convertedString + "\"";
}

bool FBDataPayload::specificPayloadEqual(const AbstractPayload& other) const {
  const auto& childInstance = dynamic_cast<const FBDataPayload&>(other); 
  return dataId == childInstance.dataId && value == childInstance.value;
}

// ********************* //
// FBInstanceDataPayload //
// ********************* //

FBInstanceDataPayload::FBInstanceDataPayload(const bt_field* field) : AbstractPayload(field){

  std::array<const char*, 4> fieldNames{"inputs", "outputs", "internal", "internalFB"};
  std::array<std::vector<std::string>*, 4> storage = {&inputs, &outputs, &internal, &internalFB};

  for(size_t fieldRunner = 0; fieldRunner < fieldNames.size(); fieldRunner++){
    auto arrayField = bt_field_structure_borrow_member_field_by_name_const(field, fieldNames[fieldRunner]);
    auto arrayLength = bt_field_array_get_length(arrayField);
    for(decltype(arrayLength) dynamicArrayRunner = 0; dynamicArrayRunner < arrayLength; dynamicArrayRunner++) {
      storage[fieldRunner]->emplace_back(bt_field_string_get_value(
          bt_field_array_borrow_element_field_by_index_const(arrayField, dynamicArrayRunner)
      ));
    } 
  }
}
  
std::string FBInstanceDataPayload::specificPayloadString() const {

  auto createStringFromVector = [](const std::vector<std::string>& vec) -> std::string {
    std::string result = "[";
    for(size_t i = 0; i < vec.size(); i++) {
      if(i != 0) {
        result += ",";
      }
      result += " [" + std::to_string(i) + "] = \"" + vec[i] + "\""; 
    }
    result += " ]";
    return result;
  };

  return ", _inputs_len = " + std::to_string(inputs.size()) + ", inputs = " + createStringFromVector(inputs) +
    ", _outputs_len = " + std::to_string(outputs.size()) + ", outputs = " + createStringFromVector(outputs) +
    ", _internal_len = " + std::to_string(internal.size()) + ", internal = " + createStringFromVector(internal) +
    ", _internalFB_len = " + std::to_string(internalFB.size()) + ", internalFB = " + createStringFromVector(internalFB);
}

bool FBInstanceDataPayload::specificPayloadEqual(const AbstractPayload& other) const {
  const auto& childInstance = dynamic_cast<const FBInstanceDataPayload&>(other); 
  return inputs == childInstance.inputs && outputs == childInstance.outputs && internal == childInstance.internal && internalFB == childInstance.internalFB;
}

// ************** //
// PayloadFactory //
// ************** //

std::unique_ptr<AbstractPayload> PayloadFactory::createPayload(const std::string& eventType, const bt_field* field){
  std::unique_ptr<AbstractPayload> result = nullptr;
  if(eventType == "receiveInputEvent" || eventType == "sendOutputEvent") {
    result.reset(new FBEventPayload(field));
  } else if(eventType == "inputData" || eventType == "outputData") {
    result.reset(new FBDataPayload(field));
  } else if(eventType == "instanceData") {
    result.reset(new FBInstanceDataPayload(field));
  }
  return result;
}
