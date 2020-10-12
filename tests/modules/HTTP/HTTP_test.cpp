/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>

#include "../../../src/com/HTTP/httpparser.h"

BOOST_AUTO_TEST_SUITE (HTTPParser_function_test)

  BOOST_AUTO_TEST_CASE(createGetRequest_test) {

    const char* resultValid1 = "GET / HTTP/1.1\r\nHost: 0.0.0.0\r\n\r\n";
    const char* resultValid2 = "GET / HTTP/1.1\r\nHost: 192.168.0.1\r\n\r\n";
    const char* resultValid3 = "GET /lookFor HTTP/1.1\r\nHost: 192.168.0.1\r\n\r\n";
    const char* resultValid4 = "GET /lookFor/something/else HTTP/1.1\r\nHost: 192.168.0.1\r\n\r\n";

    CIEC_STRING dest = "random text";
    CIEC_STRING host = "0.0.0.0";
    CIEC_STRING path = "/";

    forte::com_infra::CHttpParser::createGetRequest(dest, host, path);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), resultValid1));

    host = "192.168.0.1";
    forte::com_infra::CHttpParser::createGetRequest(dest, host, path);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), resultValid2));

    path = "/lookFor";
    forte::com_infra::CHttpParser::createGetRequest(dest, host, path);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), resultValid3));

    path = "/lookFor/something/else";
    forte::com_infra::CHttpParser::createGetRequest(dest, host, path);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), resultValid4));

  }

  BOOST_AUTO_TEST_CASE(PUTPOST_test) {

    const char* putResultValid1 = "PUT / HTTP/1.1\r\nHost: 0.0.0.0\r\nContent-type: text/xml\r\nContent-length: 19\r\n\r\nkey1=val1;key2=val2";
    const char* putResultValid1Changed =
      "PUT / HTTP/1.1\r\nHost: 0.0.0.0\r\nContent-type: text/xml\r\nContent-length: 33\r\n\r\nkey1=val1Changed;key2Changed=val2";
    const char* putResultValid2 = "PUT / HTTP/1.1\r\nHost: 192.168.0.1\r\nContent-type: text/xml\r\nContent-length: 19\r\n\r\nkey1=val1;key2=val2";
    const char* putResultValid3 = "PUT /lookFor HTTP/1.1\r\nHost: 192.168.0.1\r\nContent-type: text/xml\r\nContent-length: 19\r\n\r\nkey1=val1;key2=val2";
    const char* putResultValid4 =
      "PUT /lookFor/something/else HTTP/1.1\r\nHost: 192.168.0.1\r\nContent-type: text/xml\r\nContent-length: 19\r\n\r\nkey1=val1;key2=val2";

    const char* putEmptyData = "PUT /lookFor/something/else HTTP/1.1\r\nHost: 192.168.0.1\r\nContent-type: text/xml\r\nContent-length: 0\r\n\r\n";
    const char* putJSONType =
      "PUT /lookFor/something/else HTTP/1.1\r\nHost: 192.168.0.1\r\nContent-type: application/json\r\nContent-length: 29\r\n\r\n{\"key1\" : val1,\"key2\" : val2}";

    const char* postResultValid1 = "POST / HTTP/1.1\r\nHost: 0.0.0.0\r\nContent-type: text/xml\r\nContent-length: 19\r\n\r\nkey1=val1;key2=val2";
    const char* postResultValid1Changed =
      "POST / HTTP/1.1\r\nHost: 0.0.0.0\r\nContent-type: text/xml\r\nContent-length: 33\r\n\r\nkey1=val1Changed;key2Changed=val2";
    const char* postResultValid2 = "POST / HTTP/1.1\r\nHost: 192.168.0.1\r\nContent-type: text/xml\r\nContent-length: 19\r\n\r\nkey1=val1;key2=val2";
    const char* postResultValid3 = "POST /lookFor HTTP/1.1\r\nHost: 192.168.0.1\r\nContent-type: text/xml\r\nContent-length: 19\r\n\r\nkey1=val1;key2=val2";
    const char* postResultValid4 =
      "POST /lookFor/something/else HTTP/1.1\r\nHost: 192.168.0.1\r\nContent-type: text/xml\r\nContent-length: 19\r\n\r\nkey1=val1;key2=val2";

    const char* postEmptyData = "POST /lookFor/something/else HTTP/1.1\r\nHost: 192.168.0.1\r\nContent-type: text/xml\r\nContent-length: 0\r\n\r\n";

    const char* postJSONType =
      "POST /lookFor/something/else HTTP/1.1\r\nHost: 192.168.0.1\r\nContent-type: application/json\r\nContent-length: 29\r\n\r\n{\"key1\" : val1,\"key2\" : val2}";


    CIEC_STRING dest = "random text";
    CIEC_STRING destEmpty = "";
    CIEC_STRING host = "0.0.0.0";
    CIEC_STRING path = "/";
    CIEC_STRING data = "key1=val1;key2=val2";
    CIEC_STRING dataChanged = "key1=val1Changed;key2Changed=val2";
    CIEC_STRING contentType = "text/xml";

    forte::com_infra::CHttpParser::createPutPostRequest(dest, host, path, data, contentType, forte::com_infra::CHttpComLayer::e_PUT);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), putResultValid1));
    BOOST_CHECK_EQUAL(true, forte::com_infra::CHttpParser::changePutPostData(dest, dataChanged));
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), putResultValid1Changed));
    BOOST_CHECK_EQUAL(true, forte::com_infra::CHttpParser::changePutPostData(dest, data));
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), putResultValid1));

    forte::com_infra::CHttpParser::createPutPostRequest(dest, host, path, data, contentType, forte::com_infra::CHttpComLayer::e_POST);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), postResultValid1));
    BOOST_CHECK_EQUAL(true, forte::com_infra::CHttpParser::changePutPostData(dest, dataChanged));
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), postResultValid1Changed));
    BOOST_CHECK_EQUAL(true, forte::com_infra::CHttpParser::changePutPostData(dest, data));
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), postResultValid1));

    BOOST_CHECK_EQUAL(false, forte::com_infra::CHttpParser::changePutPostData(destEmpty, data));
    dataChanged = "";
    BOOST_CHECK_EQUAL(false, forte::com_infra::CHttpParser::changePutPostData(destEmpty, dataChanged));

    host = "192.168.0.1";
    forte::com_infra::CHttpParser::createPutPostRequest(dest, host, path, data, contentType, forte::com_infra::CHttpComLayer::e_PUT);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), putResultValid2));
    forte::com_infra::CHttpParser::createPutPostRequest(dest, host, path, data, contentType, forte::com_infra::CHttpComLayer::e_POST);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), postResultValid2));

    path = "/lookFor";
    forte::com_infra::CHttpParser::createPutPostRequest(dest, host, path, data, contentType, forte::com_infra::CHttpComLayer::e_PUT);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), putResultValid3));
    forte::com_infra::CHttpParser::createPutPostRequest(dest, host, path, data, contentType, forte::com_infra::CHttpComLayer::e_POST);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), postResultValid3));

    path = "/lookFor/something/else";
    forte::com_infra::CHttpParser::createPutPostRequest(dest, host, path, data, contentType, forte::com_infra::CHttpComLayer::e_PUT);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), putResultValid4));
    forte::com_infra::CHttpParser::createPutPostRequest(dest, host, path, data, contentType, forte::com_infra::CHttpComLayer::e_POST);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), postResultValid4));

    data = "";
    forte::com_infra::CHttpParser::createPutPostRequest(dest, host, path, data, contentType, forte::com_infra::CHttpComLayer::e_PUT);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), putEmptyData));
    forte::com_infra::CHttpParser::createPutPostRequest(dest, host, path, data, contentType, forte::com_infra::CHttpComLayer::e_POST);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), postEmptyData));

    contentType = "application/json";
    data = "{\"key1\" : val1,\"key2\" : val2}";
    forte::com_infra::CHttpParser::createPutPostRequest(dest, host, path, data, contentType, forte::com_infra::CHttpComLayer::e_PUT);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), putJSONType));
    forte::com_infra::CHttpParser::createPutPostRequest(dest, host, path, data, contentType, forte::com_infra::CHttpComLayer::e_POST);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), postJSONType));
  }

  BOOST_AUTO_TEST_CASE(parseResponse_test) {

    CIEC_STRING validReponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nkey1=val1;key2=val2";
    CIEC_STRING validReponseNoBody = "HTTP/1.1 200 OK\r\n";
    CIEC_STRING worngFirstLine = "HTTP/1.1 200 OK\nContent-Type: text/html";
    CIEC_STRING worngStatusLine = "HTTP/1.1 200OK\r\nContent-Type: text/html\r\n\r\nkey1=val1;key2=val2";

    CIEC_STRING body;
    CIEC_STRING responseCode;

    BOOST_CHECK_EQUAL(true, forte::com_infra::CHttpParser::parseResponse(body, responseCode, validReponse.getValue()));
    BOOST_CHECK_EQUAL(0, strcmp(body.getValue(), "key1=val1;key2=val2"));
    BOOST_CHECK_EQUAL(0, strcmp(responseCode.getValue(), "200"));

    responseCode = "";
    BOOST_CHECK_EQUAL(true, forte::com_infra::CHttpParser::parseResponse(body, responseCode, validReponseNoBody.getValue()));
    BOOST_CHECK_EQUAL(0, strcmp(body.getValue(), ""));
    BOOST_CHECK_EQUAL(0, strcmp(responseCode.getValue(), "200"));

    BOOST_CHECK_EQUAL(false, forte::com_infra::CHttpParser::parseResponse(body, responseCode, worngFirstLine.getValue()));
    BOOST_CHECK_EQUAL(false, forte::com_infra::CHttpParser::parseResponse(body, responseCode, worngStatusLine.getValue()));

  }

  BOOST_AUTO_TEST_CASE(createResponse_test) {

    const char* validResult = "HTTP/1.1 200 OK\r\nContent-type: application/json\r\nContent-length: 29\r\n\r\n{\"key1\" : val1,\"key2\" : val2}";
    const char* validResultNoBody = "HTTP/1.1 200 OK\r\n";

    CIEC_STRING dest;
    CIEC_STRING result = "HTTP/1.1 200 OK";
    CIEC_STRING contentType = "application/json";
    CIEC_STRING data = "{\"key1\" : val1,\"key2\" : val2}";

    forte::com_infra::CHttpParser::createResponse(dest, result, contentType, data);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), validResult));

    data = "";
    forte::com_infra::CHttpParser::createResponse(dest, result, contentType, data);
    BOOST_CHECK_EQUAL(0, strcmp(dest.getValue(), validResultNoBody));

  }

  BOOST_AUTO_TEST_CASE(parseGetRequest_test) {
    CIEC_STRING resultValidNoParameters = "GET / HTTP/1.1\r\nHost: 0.0.0.0\r\n\r\n";
    CIEC_STRING resultValidNoParameters2 = "GET /path/to/look HTTP/1.1\r\nHost: 0.0.0.0\r\n\r\n";
    CIEC_STRING resultValidParam = "GET /?key1=val1 HTTP/1.1\r\nHost: 0.0.0.0\r\n\r\n";
    CIEC_STRING resultValidParams = "GET /?key1=val1&key2=val2 HTTP/1.1\r\nHost: 0.0.0.0\r\n\r\n";
    CIEC_STRING resultValidParam2 = "GET /path/to/look?key1=val1 HTTP/1.1\r\nHost: 0.0.0.0\r\n\r\n";
    CIEC_STRING resultValidParams2 = "GET /path/to/look?key1=val1&key2=val2 HTTP/1.1\r\nHost: 0.0.0.0\r\n\r\n";

    CIEC_STRING invalidGET = "Get / HTTP/1.1\r\nHost: 0.0.0.0\r\n\r\n";
    CIEC_STRING invalidPath = "GET /pathHTTP/1.1\r\n";

    CIEC_STRING path;
    CSinglyLinkedList<CIEC_STRING> parameterNames;
    CSinglyLinkedList<CIEC_STRING> parameterValues;

    BOOST_CHECK_EQUAL(true, forte::com_infra::CHttpParser::parseGetRequest(path, parameterNames, parameterValues, resultValidNoParameters.getValue()));
    BOOST_CHECK_EQUAL(0, strcmp(path.getValue(), "/"));
    BOOST_CHECK_EQUAL(true, parameterNames.isEmpty());
    BOOST_CHECK_EQUAL(true, parameterValues.isEmpty());

    BOOST_CHECK_EQUAL(true, forte::com_infra::CHttpParser::parseGetRequest(path, parameterNames, parameterValues, resultValidNoParameters2.getValue()));
    BOOST_CHECK_EQUAL(0, strcmp(path.getValue(), "/path/to/look"));
    BOOST_CHECK_EQUAL(true, parameterNames.isEmpty());
    BOOST_CHECK_EQUAL(true, parameterValues.isEmpty());

    BOOST_CHECK_EQUAL(true, forte::com_infra::CHttpParser::parseGetRequest(path, parameterNames, parameterValues, resultValidParam.getValue()));
    BOOST_CHECK_EQUAL(0, strcmp(path.getValue(), "/"));

    CSinglyLinkedList<CIEC_STRING>::Iterator namesIter = parameterNames.begin();
    CSinglyLinkedList<CIEC_STRING>::Iterator valuesIter = parameterValues.begin();

    BOOST_CHECK_EQUAL(0, strcmp((*namesIter).getValue(), "key1"));
    BOOST_CHECK_EQUAL(0, strcmp((*valuesIter).getValue(), "val1"));

    parameterNames.clearAll();
    parameterValues.clearAll();

    BOOST_CHECK_EQUAL(true, forte::com_infra::CHttpParser::parseGetRequest(path, parameterNames, parameterValues, resultValidParams.getValue()));
    BOOST_CHECK_EQUAL(0, strcmp(path.getValue(), "/"));

    namesIter = parameterNames.begin();
    valuesIter = parameterValues.begin();

    BOOST_CHECK_EQUAL(0, strcmp((*namesIter).getValue(), "key1"));
    BOOST_CHECK_EQUAL(0, strcmp((*valuesIter).getValue(), "val1"));
    ++namesIter;
    ++valuesIter;
    BOOST_CHECK_EQUAL(0, strcmp((*namesIter).getValue(), "key2"));
    BOOST_CHECK_EQUAL(0, strcmp((*valuesIter).getValue(), "val2"));

    parameterNames.clearAll();
    parameterValues.clearAll();

    BOOST_CHECK_EQUAL(true, forte::com_infra::CHttpParser::parseGetRequest(path, parameterNames, parameterValues, resultValidParam2.getValue()));
    BOOST_CHECK_EQUAL(0, strcmp(path.getValue(), "/path/to/look"));

    namesIter = parameterNames.begin();
    valuesIter = parameterValues.begin();

    BOOST_CHECK_EQUAL(0, strcmp((*namesIter).getValue(), "key1"));
    BOOST_CHECK_EQUAL(0, strcmp((*valuesIter).getValue(), "val1"));

    parameterNames.clearAll();
    parameterValues.clearAll();

    BOOST_CHECK_EQUAL(true, forte::com_infra::CHttpParser::parseGetRequest(path, parameterNames, parameterValues, resultValidParams2.getValue()));
    BOOST_CHECK_EQUAL(0, strcmp(path.getValue(), "/path/to/look"));

    namesIter = parameterNames.begin();
    valuesIter = parameterValues.begin();

    BOOST_CHECK_EQUAL(0, strcmp((*namesIter).getValue(), "key1"));
    BOOST_CHECK_EQUAL(0, strcmp((*valuesIter).getValue(), "val1"));
    ++namesIter;
    ++valuesIter;
    BOOST_CHECK_EQUAL(0, strcmp((*namesIter).getValue(), "key2"));
    BOOST_CHECK_EQUAL(0, strcmp((*valuesIter).getValue(), "val2"));

    BOOST_CHECK_EQUAL(false, forte::com_infra::CHttpParser::parseGetRequest(path, parameterNames, parameterValues, invalidGET.getValue()));
    BOOST_CHECK_EQUAL(false, forte::com_infra::CHttpParser::parseGetRequest(path, parameterNames, parameterValues, invalidPath.getValue()));

  }

  BOOST_AUTO_TEST_SUITE_END()
