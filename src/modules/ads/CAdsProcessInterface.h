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
 *    Martin Melik-Merkumians - adds Byte and LWord functions
 *******************************************************************************/

#ifndef SRC_MODULES_ADS_CADSPROCESSINTERFACE_H_
#define SRC_MODULES_ADS_CADSPROCESSINTERFACE_H_

#include "../../stdfblib/io/processinterfacebase.h"
#include "CAdsConnectionManager.h"

namespace forte {
  namespace ads {

    class CAdsProcessInterface : public CProcessInterfaceBase{
      public:
        CAdsProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData);
        virtual ~CAdsProcessInterface();

      protected:
        bool initialise(bool paIsInput);
        bool deinitialise();
        bool writePin();
        bool readPin();
        bool writeByte();
        bool readByte();
        bool writeWord();
        bool readWord();
        bool writeDWord();
        bool readDWord();
#ifdef FORTE_USE_64BIT_DATATYPES
        bool writeLWord();
        bool readLWord();
#endif /* FORTE_USE_64BIT_DATATYPES */

      private:
        std::string mFriendlyAdsServerName;
        std::string mAdsVariableName;
        uint32_t mAdsHandle;
    };
  }
}
#endif /* SRC_MODULES_ADS_CADSPROCESSINTERFACE_H_ */
