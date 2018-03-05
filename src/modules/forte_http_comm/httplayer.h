/*******************************************************************************
*
********************************************************************************
*******************************************************************************
* Copyright (c) 2017-2018 Marc Jakobi, github.com/MrcJkb
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*
* Contributors:
*    Marc Jakobi - initial implementation for HTTP clients
*******************************************************************************
******************************************************************************
*
*
********************************************************************************
* HTTP Com Layer
********************************************************************************/

#ifndef _HTTPCOMLAYER_H_
#define _HTTPCOMLAYER_H_

#include <forte_config.h>
#include "comlayer.h"
#include "httpparser.h"
#include <stdint.h>

class CIEC_ANY;

namespace forte {

  namespace com_infra {

	class CHttpComLayer : public CComLayer{
	  public:
		  CHttpComLayer(CComLayer* pa_poUpperLayer, CCommFB* pa_poComFB);
		virtual ~CHttpComLayer();

		EComResponse sendData(void *pa_pvData, unsigned int pa_unSize); // top interface, called from top
		EComResponse recvData(const void *pa_pvData, unsigned int pa_unSize);

		void closeConnection();

		/** Size with which to allocate char arrays */
		static const size_t kAllocSize = 512;


	  private:
		EComResponse openConnection(char *pa_acLayerParameter);
		EComResponse openConnection();
		/** Serializes the data to a char* */
		bool serializeData(const CIEC_ANY& pa_roCIECData);

		/** enum representing the HTTP request type */
		enum ERequestType {
			/** HTTP GET */
			e_GET,
			/** HTTP PUT */
			e_PUT,
			/** not ready */
			e_NOTSET,
		};

		/** Handles the parsing of HTTP requests */
		CHttpParser mHttpParser;
		/** Represents the HTTP request type (0 = GET, 1 = PUT). */
		ERequestType m_eRequestType;
		/** HTTP connection parameters */
		char mParams[kAllocSize];
		/** Data to be sent */
		char mReqData[kAllocSize];
	};

  }

}

#endif /* _HTTPCOMLAYER_H_ */
