/*******************************************************************************
 * Copyright (c) 2018 TU Wien/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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
