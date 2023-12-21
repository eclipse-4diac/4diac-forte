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

BOOST_AUTO_TEST_CASE(emptyId_test) {

  const char* stringToTest = "";

  char* remainingId = new char[strlen(stringToTest) + 1];
  char *remainingIdToDelete = remainingId;

  memcpy(remainingId, stringToTest, strlen(stringToTest) + 1);
  char* layerParams = nullptr;
  char* layerId = nullptr;

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(layerId == nullptr);

  delete[] remainingIdToDelete;

}

BOOST_AUTO_TEST_CASE(contained_bracket_test) {

  const char* stringToTest = "layer[param[subparam]]";

  char* remainingId = new char[strlen(stringToTest) + 1];
  char *remainingIdToDelete = remainingId;

  memcpy(remainingId, stringToTest, strlen(stringToTest) + 1);
  char* layerParams = nullptr;
  char* layerId = nullptr;

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(layerId != nullptr);
  BOOST_CHECK_EQUAL("layer", layerId);
  BOOST_CHECK(layerParams != nullptr);
  BOOST_CHECK_EQUAL("param[subparam]", layerParams);
  BOOST_CHECK(remainingId != nullptr);
  BOOST_CHECK_EQUAL("", remainingId);


  delete[] remainingIdToDelete;

}


BOOST_AUTO_TEST_CASE(noOpeningBracket_test) {

  const char* stringToTest = "id1]";

  char* remainingId = new char[strlen(stringToTest) + 1];
  char *remainingIdToDelete = remainingId;

  memcpy(remainingId, stringToTest, strlen(stringToTest) + 1);
  char* layerParams = nullptr;
  char* layerId = nullptr;

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(layerId == nullptr);

  delete[] remainingIdToDelete;
}


BOOST_AUTO_TEST_CASE(noClosingBracket_test) {

  const char* stringToTest = "id1[";

  char* remainingId = new char[strlen(stringToTest) + 1];
  char *remainingIdToDelete = remainingId;

  memcpy(remainingId, stringToTest, strlen(stringToTest) + 1);
  char* layerParams = nullptr;
  char* layerId = nullptr;

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(layerId == nullptr);

  delete[] remainingIdToDelete;

}

BOOST_AUTO_TEST_CASE(noId_test) {

  const char* stringToTest = "[]";

  char* remainingId = new char[strlen(stringToTest) + 1];
  char *remainingIdToDelete = remainingId;

  memcpy(remainingId, stringToTest, strlen(stringToTest) + 1);
  char* layerParams = nullptr;
  char* layerId = nullptr;

  layerId = CExtractLayerAndParamsCommFB::extractLayerIdAndParams(&remainingId, &layerParams);

  BOOST_CHECK(0 == strcmp(layerId, ""));
  BOOST_CHECK(0 == strcmp(layerParams, ""));
  BOOST_CHECK(0 == strcmp(remainingId, ""));

  delete[] remainingIdToDelete;

}

BOOST_AUTO_TEST_SUITE_END()
