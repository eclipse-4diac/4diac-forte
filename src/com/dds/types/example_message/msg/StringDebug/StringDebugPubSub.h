/*******************************************************************************
 * Copyright (c) 2024 OFFIS e.V.
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Malte Grave - initial implementation
 ********************************************************************************/

#ifndef _COM_DDS_TYPES_EXAMPLE_MSG_MSG_STRINGDEBUG_PUBLISHER_H_
#define _COM_DDS_TYPES_EXAMPLE_MSG_MSG_STRINGDEBUG_PUBLISHER_H_

#include "ddspubsub.h"
#include "StringDebugPubSubTypes.h"

#include <memory>


namespace example_message {

class StringDebugPubSub : public CDDSPubSub {
  public:
    StringDebugPubSub(std::string topicName, EPubSubRole role) : 
      CDDSPubSub(topicName, role),
      m_type(new example_message::msg::StringDebugPubSubType()) {}

    std::string registerType(DomainParticipant* paParticipant) override;
    bool validateType(const CStringDictionary::TStringId typeId) override;
    bool publish(CIEC_STRUCT* data) override;
    std::optional<std::unique_ptr<CIEC_STRUCT>> receive() override;

  private:
    TypeSupport m_type;
};

}

#endif
