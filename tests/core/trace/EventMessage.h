#ifndef _FORTE_TESTS_CORE_TRACE_EVENT_MESSAGE_H_
#define _FORTE_TESTS_CORE_TRACE_EVENT_MESSAGE_H_

#include <memory>
#include <string>
#include <vector>

#include <babeltrace2/babeltrace.h>

class AbstractPayload;

class EventMessage {
public:
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
  AbstractPayload(const bt_field* field);
  
  virtual std::string getString() const;

  virtual bool operator==(const AbstractPayload& other) const;

protected:
  virtual bool specificPayloadEqual(const AbstractPayload& other) const = 0;

  virtual std::string specificPayloadString() const = 0;

  std::string mTypeName;
  std::string instanceName;
};



class FBEventPayload : public AbstractPayload {
public:
  FBEventPayload(const bt_field* field);

private:
  std::string specificPayloadString() const;

  bool specificPayloadEqual(const AbstractPayload& other) const;
 
  uint64_t eventId;
};

class FBDataPayload : public AbstractPayload {
public:
  FBDataPayload(const bt_field* field);

private:
  std::string specificPayloadString() const;

  bool specificPayloadEqual(const AbstractPayload& other) const;

  uint64_t dataId;
  std::string value;
};

class FBInstanceDataPayload : public AbstractPayload {
public:
  FBInstanceDataPayload(const bt_field* field);
  
private:
  std::string specificPayloadString() const;

  bool specificPayloadEqual(const AbstractPayload& other) const;

  std::vector<std::string> inputs;
  std::vector<std::string> outputs;
  std::vector<std::string> internal;
  std::vector<std::string> internalFB;
};

class PayloadFactory {
public:
  static std::unique_ptr<AbstractPayload> createPayload(const std::string& eventType, const bt_field* field);
};



#endif //  _FORTE_TESTS_CORE_TRACE_EVENT_MESSAGE_H_