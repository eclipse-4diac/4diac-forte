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

#ifndef SRC_MODULES_ADS_CADSCONNECTION_H_
#define SRC_MODULES_ADS_CADSCONNECTION_H_

#include "AdsLib.h"
#include <string.h>
#include <stdint.h>

namespace forte {
  namespace ads {

    class CAdsConnection{
      public:
        CAdsConnection(const std::string& paAddr, const std::string& paRemoteIpOrHostName);
        CAdsConnection(const std::string& paAddr, uint16_t paPort, const std::string& paRemoteIpOrHostName);
        virtual ~CAdsConnection();
        bool connect();

        uint16_t getPort() const;
        const AmsAddr * const getRemoteDevice() const;

      private:
        AmsAddr mRemoteDevice;
        std::string mRemoteIpV4OrHostName;
        uint16_t mPort;
    };

  } /* namespace ads */
} /* namespace forte */

#endif /* SRC_MODULES_ADS_CADSCONNECTION_H_ */
