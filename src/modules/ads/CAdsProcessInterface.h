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
        bool writeWord();
        bool readWord();
        bool writeDWord();
        bool readDWord();

      private:
        std::string mFriendlyAdsServerName;
        std::string mAdsVariableName;
        uint32_t mAdsHandle;
    };
  }
}
#endif /* SRC_MODULES_ADS_CADSPROCESSINTERFACE_H_ */
