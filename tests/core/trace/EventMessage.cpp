#include "EventMessage.h"

#include <array>
#include <iomanip>
#include <iostream>

// ******************** //
// EventMessage methods //
// ******************** //

EventMessage::EventMessage(std::string paEventType, std::unique_ptr<AbstractPayload> paPayload, int64_t paTimestamp) : 
  mEventType(std::move(paEventType)), mPayload(std::move(paPayload)), mTimestamp(paTimestamp){
}

EventMessage::EventMessage(const bt_message* paMessage)  {
  
  // Borrow the event message's event and its class
  const bt_event *event =
    bt_message_event_borrow_event_const(paMessage);
  const bt_event_class *event_class = bt_event_borrow_class_const(event);

  const bt_field *payload_field =
      bt_event_borrow_payload_field_const(event);

  mEventType = bt_event_class_get_name(event_class);
  mPayload = PayloadFactory::createPayload(mEventType, payload_field);

  if(mPayload == nullptr){
    std::cout << "Payload for message of " << mEventType << " type could not be created" << std::endl;
    std::abort();
  }

  if(BT_CLOCK_SNAPSHOT_GET_NS_FROM_ORIGIN_STATUS_OK != 
      bt_clock_snapshot_get_ns_from_origin(
        bt_message_event_borrow_default_clock_snapshot_const(paMessage), 
        &mTimestamp)){
    std::cout << "Could not retrieve clock for message" << std::endl;
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

bool EventMessage::operator==(const EventMessage& paOther) const {
  return mEventType == paOther.mEventType  && 
    (mPayload == nullptr) ? (paOther == nullptr) : (paOther.mPayload != nullptr && *mPayload == *paOther.mPayload);
}

// *********************** //
// AbstractPayload Methods //
// *********************** //
AbstractPayload::AbstractPayload(std::string paTypeName, std::string paInstanceName) 
  : mTypeName(std::move(paTypeName)), mInstanceName(std::move(paInstanceName))  {
}

AbstractPayload::AbstractPayload(const bt_field* paField) {
  auto const typeNameField = bt_field_structure_borrow_member_field_by_name_const(paField, "typeName");
  if(typeNameField == nullptr){
    std::cout << "Could not find 'typeName' member in field" << std::endl;
    std::abort();
  } else if (BT_FIELD_CLASS_TYPE_STRING != bt_field_get_class_type (typeNameField)){
    std::cout << "typeName field is not of type string" << std::endl;
    std::abort();
  }

  auto const instanceNameField = bt_field_structure_borrow_member_field_by_name_const(paField, "instanceName");
  if(instanceNameField == nullptr){
    std::cout << "Could not find 'instanceName' member in field" << std::endl;
    std::abort();
  } else if (BT_FIELD_CLASS_TYPE_STRING != bt_field_get_class_type (instanceNameField)){
    std::cout << "instanceName field is not of type string" << std::endl;
    std::abort();
  }

  mTypeName = bt_field_string_get_value(typeNameField);
  mInstanceName = bt_field_string_get_value(instanceNameField);
}

std::string AbstractPayload::getString() const {
  return "{ typeName = \"" + mTypeName + "\", instanceName = \"" + mInstanceName + "\"" + specificPayloadString() + " }";
}

bool AbstractPayload::operator==(const AbstractPayload& paOther) const {
  return mTypeName == paOther.mTypeName && mInstanceName == paOther.mInstanceName && specificPayloadEqual(paOther);
}

// ************** //
// FBEventPayload //
// ************** //
FBEventPayload::FBEventPayload(std::string paTypeName, std::string paInstanceName, const uint64_t paEventId) : 
  AbstractPayload(std::move(paTypeName), std::move(paInstanceName)),
  mEventId(paEventId) {
}

FBEventPayload::FBEventPayload(const bt_field* paField) : AbstractPayload(paField){
  auto eventField = bt_field_structure_borrow_member_field_by_name_const(paField, "eventId");
  if(eventField == nullptr){
    std::cout << "Could not find 'eventId' member in field" << std::endl;
    std::abort();
  } else if (BT_FIELD_CLASS_TYPE_UNSIGNED_INTEGER != bt_field_get_class_type(eventField)){
    std::cout << "eventId field is not of type unsigned integer" << std::endl;
    std::abort();
  }
  mEventId = bt_field_integer_unsigned_get_value(eventField);
}

std::string FBEventPayload::specificPayloadString() const {
  return ", eventId = " + std::to_string(mEventId);
}

bool FBEventPayload::specificPayloadEqual(const AbstractPayload& paOther) const {
  const auto& childInstance = dynamic_cast<const FBEventPayload&>(paOther); 
  return mEventId == childInstance.mEventId;
}

// ************* //
// FBDataPayload //
// ************* //
FBDataPayload::FBDataPayload(std::string paTypeName, std::string paInstanceName, uint64_t paDataId, std::string paValue) :
  AbstractPayload(std::move(paTypeName), std::move(paInstanceName)),
  mDataId(paDataId), mValue(paValue){
}

FBDataPayload::FBDataPayload(const bt_field* paField) : AbstractPayload(paField){
  auto dataField = bt_field_structure_borrow_member_field_by_name_const(paField, "dataId");
  if(dataField == nullptr){
    std::cout << "Could not find 'dataId' member in field" << std::endl;
    std::abort();
  } else if (BT_FIELD_CLASS_TYPE_UNSIGNED_INTEGER != bt_field_get_class_type(dataField)){
    std::cout << "dataId field is not of type unsigned integer" << std::endl;
    std::abort();
  }

  mDataId = bt_field_integer_unsigned_get_value(dataField);

  auto valueField = bt_field_structure_borrow_member_field_by_name_const(paField, "value");
  if(valueField == nullptr){
    std::cout << "Could not find 'value' member in field" << std::endl;
    std::abort();
  } else if (BT_FIELD_CLASS_TYPE_STRING != bt_field_get_class_type(valueField)){
    std::cout << "value field is not of type unsigned string" << std::endl;
    std::abort();
  }

  mValue = bt_field_string_get_value(valueField);
}

std::string FBDataPayload::specificPayloadString() const {
  auto convertedString = mValue;

  // add backslash to quoutes and double quotes
  for(auto toReplace : {"\"", "\'"}){
    for(auto pos = convertedString.find(toReplace); pos != std::string::npos; pos = convertedString.find(toReplace, pos + 2)){
      convertedString.insert(pos, "\\");
    }
  }
  return ", dataId = " + std::to_string(mDataId) + ", value = \"" + convertedString + "\"";
}

bool FBDataPayload::specificPayloadEqual(const AbstractPayload& paOther) const {
  const auto& childInstance = dynamic_cast<const FBDataPayload&>(paOther); 
  return mDataId == childInstance.mDataId && mValue == childInstance.mValue;
}

// ********************* //
// FBInstanceDataPayload //
// ********************* //
FBInstanceDataPayload::FBInstanceDataPayload(std::string paTypeName, std::string paInstanceName, const std::vector<std::string>& paInputs,
    const std::vector<std::string>& paOutputs, const std::vector<std::string>& paInternal, const std::vector<std::string>& paInternalFB) : 
      AbstractPayload(std::move(paTypeName), std::move(paInstanceName)),
      mInputs(paInputs), mOutputs(paOutputs), mInternal(paInternal), mInternalFB(paInternalFB)
{
}

FBInstanceDataPayload::FBInstanceDataPayload(const bt_field* paField) : AbstractPayload(paField){
  readDynamicArrayField(paField, "inputs", mInputs);
  readDynamicArrayField(paField, "outputs", mOutputs);
  readDynamicArrayField(paField, "internal", mInternal);
  readDynamicArrayField(paField, "internalFB", mInternalFB);
}

void FBInstanceDataPayload::readDynamicArrayField(const bt_field* paField, const char* paFieldName, std::vector<std::string>& paStorage){
  auto arrayField = bt_field_structure_borrow_member_field_by_name_const(paField, paFieldName);
  if(arrayField == nullptr){
    std::cout << "Could not find '" << paFieldName << "' member in field" << std::endl;
    std::abort();
  } else if (BT_FIELD_CLASS_TYPE_DYNAMIC_ARRAY_WITH_LENGTH_FIELD != bt_field_get_class_type(arrayField)){
    std::cout <<  paFieldName << " field is not of type array" << std::endl;
    std::abort();
  }

  auto arrayLength = bt_field_array_get_length(arrayField);
  for(decltype(arrayLength) dynamicArrayRunner = 0; dynamicArrayRunner < arrayLength; dynamicArrayRunner++) {
    const auto arrayElementField = bt_field_array_borrow_element_field_by_index_const(arrayField, dynamicArrayRunner);
 
    if (BT_FIELD_CLASS_TYPE_STRING != bt_field_get_class_type(arrayElementField)){
      std::cout << "element of array " << paFieldName << "is not of type string" << std::endl;
      std::abort();
    }
    paStorage.emplace_back(bt_field_string_get_value(arrayElementField));
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

  return ", _inputs_len = " + std::to_string(mInputs.size()) + ", inputs = " + createStringFromVector(mInputs) +
    ", _outputs_len = " + std::to_string(mOutputs.size()) + ", outputs = " + createStringFromVector(mOutputs) +
    ", _internal_len = " + std::to_string(mInternal.size()) + ", internal = " + createStringFromVector(mInternal) +
    ", _internalFB_len = " + std::to_string(mInternalFB.size()) + ", internalFB = " + createStringFromVector(mInternalFB);
}

bool FBInstanceDataPayload::specificPayloadEqual(const AbstractPayload& paOther) const {
  const auto& childInstance = dynamic_cast<const FBInstanceDataPayload&>(paOther); 
  return mInputs == childInstance.mInputs 
        && mOutputs == childInstance.mOutputs 
        && mInternal == childInstance.mInternal 
        && mInternalFB == childInstance.mInternalFB;
}

// ************** //
// PayloadFactory //
// ************** //
std::unique_ptr<AbstractPayload> PayloadFactory::createPayload(const std::string& paEventType, const bt_field* paField){
  std::unique_ptr<AbstractPayload> result = nullptr;
  if(paEventType == "receiveInputEvent" || paEventType == "sendOutputEvent") {
    result.reset(new FBEventPayload(paField));
  } else if(paEventType == "inputData" || paEventType == "outputData") {
    result.reset(new FBDataPayload(paField));
  } else if(paEventType == "instanceData") {
    result.reset(new FBInstanceDataPayload(paField));
  }
  return result;
}

