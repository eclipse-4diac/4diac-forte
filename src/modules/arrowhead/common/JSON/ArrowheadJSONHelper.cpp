/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "ArrowheadJSONHelper.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "ArrowheadJSONHelper_gen.cpp"
#endif
#include <forte_bool.h>
#include <parameterParser.h>
#include <devlog.h>

void ArrowheadJSONHelper::transformANYToJSON(const CIEC_ANY& paSource, CIEC_STRING& paResult) {
  switch(paSource.getDataTypeID()){
    case CIEC_ANY::e_STRUCT:
      if(g_nStringIdArrowheadService == static_cast<const CIEC_STRUCT&>(paSource).getStructTypeNameID()) {
        transformServiceToJSON(const_cast<CIEC_ArrowheadService&>(static_cast<const CIEC_ArrowheadService&>(paSource)), paResult);
      } else if(g_nStringIdServiceRequestForm == static_cast<const CIEC_STRUCT&>(paSource).getStructTypeNameID()) {
        transformOrchServciceRequestFormToJSON(const_cast<CIEC_ServiceRequestForm&>(static_cast<const CIEC_ServiceRequestForm&>(paSource)), paResult);
      } else {
        transformStructToJSON(static_cast<const CIEC_STRUCT&>(paSource), paResult);
      }
      break;
    case CIEC_ANY::e_ARRAY:
      transformArrayToJSON(static_cast<const CIEC_ARRAY&>(paSource), paResult);
      break;
    case CIEC_ANY::e_BOOL:
      paResult.append(static_cast<bool>(static_cast<const CIEC_BOOL&>(paSource)) ? "true" : "false");
      break;
    default: {
      unsigned int size = paSource.getToStringBufferSize();
      char *toStore = new char[size];
      paSource.toString(toStore, size);
      paResult.append(toStore);
      delete[] toStore;
      break;
    }
  }

  //change ' from STRING to " in JSON
  char* resultString = paResult.getValue();
  for(unsigned int i = 0; i < paResult.length(); i++) {
    if('\'' == resultString[i]) {
      resultString[i] = '"';
    }
  }
}

void ArrowheadJSONHelper::transformJSONToStruct(char* paToChange) {
  transformServiceMetadata(paToChange);
  removeIds(paToChange);
  char* runner = paToChange;
  changeKeys(&runner);
  changeToStruct(paToChange);
}

void ArrowheadJSONHelper::transformStructToJSON(const CIEC_STRUCT& paSource, CIEC_STRING& paResult) {

  TForteUInt16 unSize = paSource.getStructSize();
  const CStringDictionary::TStringId *punMemberNameIds = paSource.elementNames();
  const CIEC_ANY *poMembers = paSource.getMembers();

  paResult.append("{");

  for(unsigned int i = 0; i < unSize; ++i) {
    const char *acMemberName = CStringDictionary::getInstance().get(punMemberNameIds[i]);

    paResult.append("\"");
    paResult.append(acMemberName);
    paResult.append("\":");

    transformANYToJSON(poMembers[i], paResult);

    if(i != static_cast<unsigned int>(unSize - 1)) {
      paResult.append(",");
    }
  }
  paResult.append("}");
}

void ArrowheadJSONHelper::transformServiceToJSON(CIEC_ArrowheadService& paSource, CIEC_STRING& paResult) {

  paResult.append("{\"serviceDefinition\":\"");
  paResult.append(paSource.serviceDefinition().getValue());
  paResult.append("\",\"interfaces\":");
  transformANYToJSON(*paSource.interfaces_array(), paResult);
  paResult.append(",\"serviceMetadata\":{");

  bool first = true;
  for(size_t i = 0; i < 10; i++) {
    if(0 != strcmp(paSource.serviceMetadata()[i].getValue(), "")) { //if a service is empty, keep looking for others, just in case
      if(!first) {
        paResult.append(",");
      } else {
        first = false;
      }

      CParameterParser parser(paSource.serviceMetadata()[i].getValue(), '=', 2);
      if(2 != parser.parseParameters()) {
        DEVLOG_ERROR("[ArrowheadJSONHelper]: wrong metadata %s. Each metadata should be in the form of key=value\n", paSource.serviceMetadata()[i].getValue());
        break;
      }

      paResult.append("\"");
      paResult.append(parser[0]);
      paResult.append("\":\"");
      paResult.append(parser[1]);
      paResult.append("\"");
    }
  }
  paResult.append("}}");
}

void ArrowheadJSONHelper::transformOrchServciceRequestFormToJSON(CIEC_ServiceRequestForm& paSource, CIEC_STRING& paResult) {

  paResult.append("{\"requesterSystem\":");
  transformANYToJSON(paSource.requesterSystem(), paResult);

  //Cloud shoudn't be sent by applications

  paResult.append(",\"requestedService\":");
  transformANYToJSON(paSource.requestedService(), paResult);

  paResult.append(",\"orchestrationFlags\":{");
  bool first = true;
  for(size_t i = 0; i < 10; i++) {
    if(0 != strcmp(paSource.orchestrationFlags()[i].getValue(), "")) {
      if(!first) {
        paResult.append(",");
      } else {
        first = false;
      }

      paResult.append("\"");
      paResult.append(paSource.orchestrationFlags()[i].getValue());
      paResult.append("\":true");
    } else {
      break;
    }
  }

  paResult.append("},\"preferredProviders\":[");
  first = true;
  for(size_t i = 0; i < 10; i++) {
    if(0 != strcmp(paSource.preferredProviders()[i].providerSystem().systemName().getValue(), "")) {
      if(!first) {
        paResult.append(",");
      } else {
        first = false;
      }
      transformANYToJSON(paSource.preferredProviders()[i], paResult);
    } else {
      break;
    }
  }

  paResult.append("],\"requestedQoS\":{}}"); //requestedQoS not implemented yet in the arrowhead definition
}

void ArrowheadJSONHelper::transformArrayToJSON(const CIEC_ARRAY& paSource, CIEC_STRING& paResult) {
  TForteUInt16 unSize = paSource.size();
  paResult.append("[");

  bool first = true;
  for(unsigned int i = 0; i < unSize; ++i) {
    if((CIEC_ANY::e_STRING == paSource.getElementDataTypeID() || CIEC_ANY::e_WSTRING == paSource.getElementDataTypeID())
      && 0 == strcmp(static_cast<const CIEC_WSTRING*>(paSource[i])->getValue(), "")) {
      break;
    }

    if(!first) {
      paResult.append(",");
    } else {
      first = false;
    }
    transformANYToJSON(*paSource[i], paResult);
  }
  paResult.append("]");
}

void ArrowheadJSONHelper::transformServiceMetadata(char* paText) {
  char* helper = strstr(paText, "\"serviceMetadata\"");
  while(0 != helper) {
    helper = strchr(helper, '{');
    if(0 != helper) {
      *helper = '[';

      do {
        while('"' != *helper && '}' != *helper) { //look for start of key
          helper++;
        }
        if('}' == *helper) { //end of metadata
          break;
        }
        char *startOfKey = helper;
        helper++;
        while('"' != *helper) { //look for end of key
          helper++;
        }
        char *endOfKey = helper - 1;
        helper++;
        while('"' != *helper) { //look for start of value
          helper++;
        }

        char* startOfValue = helper;
        *startOfValue = '=';
        startOfValue--;
        while(endOfKey != startOfKey) {
          *startOfValue-- = *endOfKey--;
        }
        *startOfValue = '"';
        while(startOfKey != startOfValue) {
          *startOfKey++ = ' ';
        }
        helper = strchr(startOfValue + 1, '"');
        helper++;
      } while('}' != *helper);

      *helper = ']';
      helper = strstr(helper, "\"serviceMetadata\"");
    } else {
      DEVLOG_ERROR("[ArrowheadJSONHelper]: when transforming serviceMetadata, no { char was found\n");
    }
  }
}

void ArrowheadJSONHelper::removeIds(char* paText) {
  do {
    paText = strstr(paText, "\"id\"");
    if(0 != paText) {
      while(',' != *paText) {
        *paText = ' ';
        paText++;
      }
      *paText = ' '; //for the last comma
      paText++;
    }
  } while(0 != paText);
}

void ArrowheadJSONHelper::changeKeysArray(char** paText) {

  do {
    while(' ' == **paText || '\r' == **paText || '\n' == **paText) {
      (*paText)++;
    }

    if('\0' == **paText) {
      return;
    } else if(']' == **paText) {
      (*paText)++;
      return;
    } else if('[' == **paText) {
      (*paText)++;
      changeKeysArray(paText);
    } else if('{' == **paText) {
      (*paText)++;
      changeKeys(paText);
    }
    (*paText)++;
  } while('\0' != **paText);

}

void ArrowheadJSONHelper::changeKeys(char** paText) {

  bool isKey = true;
  do {
    while(' ' == **paText || '\r' == **paText || '\n' == **paText) {
      (*paText)++;
    }

    if('\0' == **paText) {
      return;
    } else if('}' == **paText) {
      (*paText)++;
      return;
    } else if('[' == **paText) {
      (*paText)++;
      changeKeysArray(paText);
    } else if('{' == **paText) {
      (*paText)++;
      changeKeys(paText);
    }

    if('\0' == **paText) {
      return;
    }

    if(!isKey) {
      if('"' == **paText) {
        *paText = strchr((*paText) + 1, '"');
        (*paText)++;
      } else {
        while(',' != **paText && '}' != **paText) {
          (*paText)++;
        }
      }
      isKey = true;
      continue;
    }

    char *startOfKey = strchr(*paText, '"');
    (*paText) = strchr(startOfKey + 1, '"');
    char *startOfValue = strchr((*paText) + 1, ':') + 1;

    *startOfKey = ' ';
    **paText = ':';
    (*paText)++;
    **paText = '=';
    (*paText)++;
    while(*paText != startOfValue) {
      **paText = ' ';
      (*paText)++;
    }

    isKey = false;

  } while('\0' != **paText);

}

void ArrowheadJSONHelper::changeToStruct(char* paText) {
  while('\0' != *paText) {
    if('{' == *paText) {
      *paText = '(';
    } else if('}' == *paText) {
      *paText = ')';
    } else if('\n' == *paText || '\r' == *paText) {
      *paText = ' ';
    }
    paText++;
  }
}
