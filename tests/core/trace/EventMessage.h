#ifndef _FORTE_TESTS_CORE_TRACE_EVENT_MESSAGE_H_
#define _FORTE_TESTS_CORE_TRACE_EVENT_MESSAGE_H_

#include <memory>
#include <string>
#include <vector>

#include <babeltrace2/babeltrace.h>

class AbstractPayload;

class EventMessage {
public:
  EventMessage(const std::string& paEventType, std::unique_ptr<AbstractPayload> paPayload, int64_t paTimestamp);

  EventMessage(const bt_message* message);
  
  std::string getPayloadString() const;

  std::string getTimestampString() const;

  int64_t getTimestamp () const { return mTimestamp;}

  bool operator==(const EventMessage& other) const;

private:
  std::string mEventType;
  std::unique_ptr<AbstractPayload> mPayload;
  int64_t mTimestamp;
};

class AbstractPayload {
public:
  AbstractPayload(const std::string& paTypeName, const std::string& paInstanceName);

  AbstractPayload(const bt_field* field);
  
  virtual std::string getString() const;

  virtual bool operator==(const AbstractPayload& other) const;

protected:
  virtual bool specificPayloadEqual(const AbstractPayload& other) const = 0;

  virtual std::string specificPayloadString() const = 0;

  std::string mTypeName;
  std::string mInstanceName;
};

class FBEventPayload : public AbstractPayload {
public:
  FBEventPayload(const std::string& paTypeName, const std::string& paInstanceName, const uint64_t eventId);

  FBEventPayload(const bt_field* field);

private:
  std::string specificPayloadString() const;

  bool specificPayloadEqual(const AbstractPayload& other) const;
 
  uint64_t mEventId;
};

class FBDataPayload : public AbstractPayload {
public:
  FBDataPayload(const std::string& paTypeName, const std::string& paInstanceName, uint64_t paDataId, const std::string& paValue);

  FBDataPayload(const bt_field* field);

private:
  std::string specificPayloadString() const;

  bool specificPayloadEqual(const AbstractPayload& other) const;

  uint64_t mDataId;
  std::string mValue;
};

class FBInstanceDataPayload : public AbstractPayload {
public:
  FBInstanceDataPayload(const std::string& paTypeName, const std::string& paInstanceName, 
        const std::vector<std::string>& paInputs,
        const std::vector<std::string>& paOutputs,
        const std::vector<std::string>& paInternal,
        const std::vector<std::string>& paInternalFB);

  FBInstanceDataPayload(const bt_field* field);
  
private:
  std::string specificPayloadString() const;

  bool specificPayloadEqual(const AbstractPayload& other) const;

  std::vector<std::string> mInputs;
  std::vector<std::string> mOutputs;
  std::vector<std::string> mInternal;
  std::vector<std::string> mInternalFB;
};

class PayloadFactory {
public:
  static std::unique_ptr<AbstractPayload> createPayload(const std::string& paEventType, const bt_field* paField);
};

#endif //  _FORTE_TESTS_CORE_TRACE_EVENT_MESSAGE_H_