/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef FORDIAC_RTE_MODULES_ARROWHEAD_COMMON_HTTP_ARROWHEADJSONHELPER_H_
#define FORDIAC_RTE_MODULES_ARROWHEAD_COMMON_HTTP_ARROWHEADJSONHELPER_H_

#include <forte_struct.h>
#include <forte_string.h>
#include <forte_array.h>
#include "../forte_arrowheadservice.h"
#include "../../orchestrator/forte_servicerequestform.h"

class ArrowheadJSONHelper {
  public:
    static void transformANYToJSON(const CIEC_ANY &paSource, CIEC_STRING &paResult);

    static void transformJSONToStruct(char* paToChange);

  private:
    ArrowheadJSONHelper() {
    }
    virtual ~ArrowheadJSONHelper() {
    }

    static void transformStructToJSON(const CIEC_STRUCT &paSource, CIEC_STRING &paResult);

    static void transformArrayToJSON(const CIEC_ARRAY &paSource, CIEC_STRING &paResult);

    static void transformServiceToJSON(CIEC_ArrowheadService &paSource, CIEC_STRING &paResult);

    static void transformOrchServciceRequestFormToJSON(CIEC_ServiceRequestForm& paSource, CIEC_STRING& paResult);

    static void transformServiceMetadata(char* paText);

    static void removeIds(char* paText);

    static void changeKeys(char** paText);

    static void changeKeysArray(char** paText);

    static void changeToStruct(char* paText);
};

#endif /* FORDIAC_RTE_MODULES_ARROWHEAD_COMMON_HTTP_ARROWHEADJSONHELPER_H_ */
