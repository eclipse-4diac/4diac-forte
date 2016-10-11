/*******************************************************************************
 * Copyright (c) 2012 - 2014 Profactor GmbH, ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Matthias Plasch, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "GEN_APPEND_STRING.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_APPEND_STRING_gen.cpp"
#endif
#include <ctype.h>
#include <stdio.h>
#include <devlog.h>

DEFINE_GENERIC_FIRMWARE_FB(GEN_APPEND_STRING, g_nStringIdGEN_APPEND_STRING);

const CStringDictionary::TStringId GEN_APPEND_STRING::scm_anEventOutputNames[] = { g_nStringIdCNF };
const CStringDictionary::TStringId GEN_APPEND_STRING::scm_anEventInputNames[] = { g_nStringIdREQ };

const CStringDictionary::TStringId GEN_APPEND_STRING::scm_anDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId GEN_APPEND_STRING::scm_anDataOutputTypeIds[] = {g_nStringIdSTRING};

const TForteInt16 GEN_APPEND_STRING::scm_anEIWithIndexes[] = {0};

const TDataIOID GEN_APPEND_STRING::scm_anEOWith[] = {0, 255};
const TForteInt16 GEN_APPEND_STRING::scm_anEOWithIndexes[] = {0, -1};

//default value is set to 100 (sufficient for several data types)
const TForteInt16 GEN_APPEND_STRING::scm_maxStringBufSize = 100;

GEN_APPEND_STRING::GEN_APPEND_STRING(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
CFunctionBlock(pa_poSrcRes, 0, pa_nInstanceNameId, 0, 0){
}

GEN_APPEND_STRING::~GEN_APPEND_STRING(){
	delete[] m_anDataInputNames;
	delete[] m_anDataInputTypeIds;
	delete[] m_anEIWith;
}

void GEN_APPEND_STRING::executeEvent(int pa_nEIID){

	if(pa_nEIID < 1 && pa_nEIID > -1){
		//pointers to data inputs and data output
		CIEC_ANY *pDataInput;
		CIEC_ANY *pDataOutput = getDO(0);

		int nUsedBytes = -1;
		TForteUInt16 nBytesFree = 0;
		TForteUInt16 nCurrentLength = 0;
		TForteUInt16 nArrayElements = 0;
		TForteUInt16 nRequiredBytes = 0;
		TForteUInt16 nStringLength = 0;

		//iterate over data inputs
		for(int input_index = 0; input_index < m_nDInputs; input_index++){
			//get current data input
			pDataInput = getDI(input_index);

			//for string data inputs of type CIEC_STRING or CIEC_WSTRING
			if(pDataInput->getDataTypeID() == CIEC_ANY::e_STRING || pDataInput->getDataTypeID() == CIEC_ANY::e_WSTRING){
				//get length of input string
				nStringLength = static_cast<CIEC_ANY_STRING*>(pDataInput)->length();

				if(input_index == 0){
					//initialize output with first value to append
					(static_cast<CIEC_STRING*>(pDataOutput))->assign((static_cast<CIEC_ANY_STRING*>(pDataInput))->getValue(), nStringLength);
				}
				else{
					//append string value to output
					(static_cast<CIEC_STRING*>(pDataOutput))->append((static_cast<CIEC_ANY_STRING*>(pDataInput))->getValue(), nStringLength);
				}
			}
			//no string data type (use method toString)
			else{
				if(pDataInput->getDataTypeID() == CIEC_ANY::e_ARRAY){
					//get number of array elements
					nArrayElements = (static_cast<CIEC_ARRAY*>(pDataInput))->size();
					//number of required bytes (including brackets '[' ']' and ',' separators
					nRequiredBytes = static_cast<TForteUInt16>(nArrayElements * scm_maxStringBufSize + nArrayElements + 1);
				}
				else{
					//size for single data values
					nRequiredBytes = scm_maxStringBufSize;
				}


				if((static_cast<CIEC_STRING*>(pDataOutput))->length() != static_cast<TForteUInt16>(0) && input_index != 0){
					//if data has been written to pDataOutput and input_index does not refer to first input
					//get length and number of free bytes (unused memory)
					nCurrentLength = (static_cast<CIEC_STRING*>(pDataOutput))->length();
					nBytesFree = static_cast<TForteUInt16>((static_cast<CIEC_STRING*>(pDataOutput))->getCapacity() - nCurrentLength);

					//if there is not enough free memory ==> reserve
					if(nBytesFree < nRequiredBytes){
						//reserve currentLength + extension Length
						(static_cast<CIEC_STRING*>(pDataOutput))->reserve(static_cast<TForteUInt16>(nCurrentLength + nRequiredBytes));
					}
				}
				//if first data input or no data has yet been written
				else{
					//reset nCurrentLength and nBytesFree
					nCurrentLength = 0;
					nBytesFree = 0;
					//reserve required length
					(static_cast<CIEC_STRING*>(pDataOutput))->reserve(nRequiredBytes);
				}

				//append the value which has been converted to a string ==> enough capacity is required
				if((static_cast<CIEC_STRING*>(pDataOutput))->getCapacity() - nCurrentLength >= nRequiredBytes){
					//use toString to write the Data (observe offset nCurrentLength)
					nUsedBytes = pDataInput->toString(((static_cast<CIEC_STRING*>(pDataOutput))->getValue()) + nCurrentLength, nRequiredBytes);
					//maintain the state of the output data value string
					(static_cast<CIEC_STRING*>(pDataOutput))->assign((static_cast<CIEC_STRING*>(pDataOutput))->getValue(), static_cast<TForteUInt16>(nCurrentLength + nUsedBytes));
				}
			}
		}
		sendOutputEvent(0);
	}
}

bool GEN_APPEND_STRING::configureFB(const char *pa_acConfigString){
	bool bRetVal = false;

	m_nConfiguredFBTypeNameId = CStringDictionary::getInstance().insert(pa_acConfigString);

	const char *pcPos = strrchr(pa_acConfigString, '_');

	if(0 != pcPos){
		++pcPos;
		if('S' != *pcPos){
			//we have an underscore and it is not the first underscore after E
			m_nDInputs = static_cast<int>(forte::core::util::strtoul(pcPos,0,10));
			DEVLOG_DEBUG("DIs: %d;\n", m_nDInputs);
		}
		else{
			m_nDInputs = 0;
		}
	}
	else{
		return false;
	}

	if(m_nDInputs < 2){

		return false;
	}

	//now the number of needed eventInputs and dataOutputs are available in the integer array
	//create the eventInputs
	if(m_nDInputs < CFunctionBlock::scm_nMaxInterfaceEvents){

		//create the data inputs
		m_anDataInputNames = new CStringDictionary::TStringId[m_nDInputs];
		m_anDataInputTypeIds = new CStringDictionary::TStringId[m_nDInputs];

    generateGenericDataPointArrays("IN_", m_anDataInputTypeIds, m_anDataInputNames, m_nDInputs);

    //now create the WITH constructs...
		//the With-Indexes for the data variables
		m_anEIWith = new TDataIOID[m_nDInputs + 1]; //for inputs + '255' separators at the list end

		//in-withs
		for(int in_with = 0; in_with < m_nDInputs + 1; in_with = in_with + 1){
			if(in_with == m_nDInputs){
				//set end separator of with
				m_anEIWith[in_with] = 255;
			}
			else{
				m_anEIWith[in_with] = static_cast<TDataIOID>(in_with);
			}
		}

		//create the interface Specification
		SFBInterfaceSpecforGenerics *pstInterfaceSpec = new SFBInterfaceSpecforGenerics(static_cast<TForteUInt8>(1), scm_anEventInputNames, m_anEIWith, scm_anEIWithIndexes, static_cast<TForteUInt8>(1), scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, static_cast<TForteUInt8>(m_nDInputs), m_anDataInputNames, m_anDataInputTypeIds, static_cast<TForteUInt8>(1), scm_anDataOutputNames, scm_anDataOutputTypeIds);

		TForteByte *acFBConnData = new TForteByte[genFBConnDataSize(pstInterfaceSpec->m_nNumEOs, pstInterfaceSpec->m_nNumDIs, pstInterfaceSpec->m_nNumDOs)];
		TForteByte *acFBVarsData = new TForteByte[genFBVarsDataSize(pstInterfaceSpec->m_nNumDIs, pstInterfaceSpec->m_nNumDOs)];

		setupFBInterface(pstInterfaceSpec, acFBConnData, acFBVarsData, true);
		bRetVal = true;
	}
	return bRetVal;
}
