/*******************************************************************************
 * Copyright (c) 2019 fotiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral- initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <boost/test/unit_test.hpp>

#include "../../../src/core/cominfra/basecommfb.h"

class CExtractLayerAndParamsCommFB : public forte::com_infra::CBaseCommFB {
  public:
    static char *extractLayerIdAndParams(char **paRemainingID, char **paLayerParams) {
      return forte::com_infra::CBaseCommFB::extractLayerIdAndParams(paRemainingID, paLayerParams);
    }
};

BOOST_AUTO_TEST_SUITE (extractLayerAndParams_test)

BOOST_AUTO_TEST_CASE(noExtraSquareBrackets_test) {


  const char* stringToTest = "id[normalParams].id2[openingBracket[].id3[twoOpeningBrackets[[].id4[closingBracket\\]].id5[twoClosingBrackes\\]\\]].id6[bothBrackets[\\]].id7[bothBracketsInverted\\][]";

  char* remainingId = new char[strlen(stringToTest) + 1];
  char *remainingIdToDelete = remainingId;

  memcpy(remainingId, stringToTest, strlen(stringToTest) + 1);
  char* layerParams = 0;
  char* layerId = 0;

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(0 == strcmp(layerId, "id"));
  BOOST_CHECK(0 == strcmp(layerParams, "normalParams"));
  BOOST_CHECK(0 == strcmp(remainingId, "id2[openingBracket[].id3[twoOpeningBrackets[[].id4[closingBracket\\]].id5[twoClosingBrackes\\]\\]].id6[bothBrackets[\\]].id7[bothBracketsInverted\\][]"));

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(0 == strcmp(layerId, "id2"));
  BOOST_CHECK(0 == strcmp(layerParams, "openingBracket["));
  BOOST_CHECK(0 == strcmp(remainingId, "id3[twoOpeningBrackets[[].id4[closingBracket\\]].id5[twoClosingBrackes\\]\\]].id6[bothBrackets[\\]].id7[bothBracketsInverted\\][]"));

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(0 == strcmp(layerId, "id3"));
  BOOST_CHECK(0 == strcmp(layerParams, "twoOpeningBrackets[["));
  BOOST_CHECK(0 == strcmp(remainingId, "id4[closingBracket\\]].id5[twoClosingBrackes\\]\\]].id6[bothBrackets[\\]].id7[bothBracketsInverted\\][]"));

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(0 == strcmp(layerId, "id4"));
  BOOST_CHECK(0 == strcmp(layerParams, "closingBracket]"));
  BOOST_CHECK(0 == strcmp(remainingId, "id5[twoClosingBrackes\\]\\]].id6[bothBrackets[\\]].id7[bothBracketsInverted\\][]"));

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(0 == strcmp(layerId, "id5"));
  BOOST_CHECK(0 == strcmp(layerParams, "twoClosingBrackes]]"));
  BOOST_CHECK(0 == strcmp(remainingId, "id6[bothBrackets[\\]].id7[bothBracketsInverted\\][]"));

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(0 == strcmp(layerId, "id6"));
  BOOST_CHECK(0 == strcmp(layerParams, "bothBrackets[]"));
  BOOST_CHECK(0 == strcmp(remainingId, "id7[bothBracketsInverted\\][]"));

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(0 == strcmp(layerId, "id7"));
  BOOST_CHECK(0 == strcmp(layerParams, "bothBracketsInverted]["));
  BOOST_CHECK(0 == strcmp(remainingId, ""));

  delete[] remainingIdToDelete;
}

BOOST_AUTO_TEST_CASE(escapedSign_test) {

  const char* stringToTest = "id[\\\\].id2[\\\\\\]]";

  char* remainingId = new char[strlen(stringToTest) + 1];
  char *remainingIdToDelete = remainingId;

  memcpy(remainingId, stringToTest, strlen(stringToTest) + 1);
  char* layerParams = 0;
  char* layerId = 0;

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(0 == strcmp(layerId, "id"));
  BOOST_CHECK(0 == strcmp(layerParams, "\\"));
  BOOST_CHECK(0 == strcmp(remainingId, "id2[\\\\\\]]"));

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(0 == strcmp(layerId, "id2"));
  BOOST_CHECK(0 == strcmp(layerParams, "\\]"));
  BOOST_CHECK(0 == strcmp(remainingId, ""));

  delete[] remainingIdToDelete;
}

BOOST_AUTO_TEST_CASE(emptyId_test) {

  const char* stringToTest = "";

  char* remainingId = new char[strlen(stringToTest) + 1];
  char *remainingIdToDelete = remainingId;

  memcpy(remainingId, stringToTest, strlen(stringToTest) + 1);
  char* layerParams = 0;
  char* layerId = 0;

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(layerId == 0);

  delete[] remainingIdToDelete;

}


BOOST_AUTO_TEST_CASE(noOpeningBracket_test) {

  const char* stringToTest = "id1]";

  char* remainingId = new char[strlen(stringToTest) + 1];
  char *remainingIdToDelete = remainingId;

  memcpy(remainingId, stringToTest, strlen(stringToTest) + 1);
  char* layerParams = 0;
  char* layerId = 0;

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(layerId == 0);

  delete[] remainingIdToDelete;
}

BOOST_AUTO_TEST_CASE(noOpeningBracketWithScaped_test) {

  const char* stringToTest = "id1\\[]";

  char* remainingId = new char[strlen(stringToTest) + 1];
  char *remainingIdToDelete = remainingId;

  memcpy(remainingId, stringToTest, strlen(stringToTest) + 1);
  char* layerParams = 0;
  char* layerId = 0;

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(layerId == 0);

  delete[] remainingIdToDelete;

}

BOOST_AUTO_TEST_CASE(noClosingBracket_test) {

  const char* stringToTest = "id1[";

  char* remainingId = new char[strlen(stringToTest) + 1];
  char *remainingIdToDelete = remainingId;

  memcpy(remainingId, stringToTest, strlen(stringToTest) + 1);
  char* layerParams = 0;
  char* layerId = 0;

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(layerId == 0);

  delete[] remainingIdToDelete;

}

BOOST_AUTO_TEST_CASE(noClosingBracketWithScaped_test) {

  const char* stringToTest = "id1[\\]";

  char* remainingId = new char[strlen(stringToTest) + 1];
  char *remainingIdToDelete = remainingId;

  memcpy(remainingId, stringToTest, strlen(stringToTest) + 1);
  char* layerParams = 0;
  char* layerId = 0;

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(layerId == 0);

  delete[] remainingIdToDelete;

}

BOOST_AUTO_TEST_CASE(noId_test) {

  const char* stringToTest = "[]";

  char* remainingId = new char[strlen(stringToTest) + 1];
  char *remainingIdToDelete = remainingId;

  memcpy(remainingId, stringToTest, strlen(stringToTest) + 1);
  char* layerParams = 0;
  char* layerId = 0;

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(0 == strcmp(layerId, ""));
  BOOST_CHECK(0 == strcmp(layerParams, ""));
  BOOST_CHECK(0 == strcmp(remainingId, ""));

  delete[] remainingIdToDelete;

}

BOOST_AUTO_TEST_SUITE_END()
