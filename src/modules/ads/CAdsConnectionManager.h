/*******************************************************************************
 * Copyright (c) 2018 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians - initial contribution
 *******************************************************************************/

#ifndef SRC_MODULES_ADS_CADSCONNECTIONMANAGER_H_
#define SRC_MODULES_ADS_CADSCONNECTIONMANAGER_H_

#include <map>

#include "singlet.h"
#include "CAdsConnection.h"

namespace forte {
  namespace ads {
    class CAdsConnectionManager{
        DECLARE_SINGLETON (CAdsConnectionManager)

      public:
        bool addConnection(const std::string& paFriendlyServerName, const std::string& paAddr, const std::string& paRemoteIpOrHostName);
        bool addConnection(const std::string& paFriendlyServerName, const std::string& paAddr, uint16_t paPort, const std::string& paRemoteIpOrHostName);
        void removeConnection(const std::string& paFriendlyServerName);
        CAdsConnection *getConnection(const std::string& paFriendlyServerName);

      private:
        std::map<std::string, CAdsConnection*> mConnectionRegistry;
    };
  }
}

#endif /* SRC_MODULES_ADS_CADSCONNECTIONMANAGER_H_ */
