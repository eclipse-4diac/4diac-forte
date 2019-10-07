/*******************************************************************************
 * Copyright (c) 2012 - 2014 Profactor GmbH, ACIN
 *                      2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *******************************************************************************/
#include "GEN_APPEND_STRING.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_APPEND_STRING_gen.cpp"
#endif
#include <ctype.h>
#include <stdio.h>

DEFINE_GENERIC_FIRMWARE_FB(GEN_APPEND_STRING, g_nStringIdGEN_APPEND_STRING);

const CStringDictionary::TStringId GEN_APPEND_STRING::scm_anEventOutputNames[] = { g_nStringIdCNF };
const CStringDictionary::TStringId GEN_APPEND_STRING::scm_anEventInputNames[] = { g_nStringIdREQ };

const CStringDictionary::TStringId GEN_APPEND_STRING::scm_anDataOutputNames[] = { g_nStringIdOUT };
const CStringDictionary::TStringId GEN_APPEND_STRING::scm_anDataOutputTypeIds[] = { g_nStringIdSTRING };

const TForteInt16 GEN_APPEND_STRING::scm_anEIWithIndexes[] = { 0 };

const TDataIOID GEN_APPEND_STRING::scm_anEOWith[] = { 0, 255 };
const TForteInt16 GEN_APPEND_STRING::scm_anEOWithIndexes[] = { 0, -1 };

//default value is set to 100 (sufficient for several data types)
const TForteInt16 GEN_APPEND_STRING::scm_maxStringBufSize = 100;

GEN_APPEND_STRING::GEN_APPEND_STRING(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(paSrcRes, paInstanceNameId), m_anDataInputNames(0), m_anDataInputTypeIds(0), m_anEIWith(0), mDInputs(0){
}

GEN_APPEND_STRING::~GEN_APPEND_STRING(){
  delete[] m_anDataInputNames;
  delete[] m_anDataInputTypeIds;
  delete[] m_anEIWith;
}

void GEN_APPEND_STRING::executeEvent(int paEIID){

  if(paEIID < 1 && paEIID > -1){
    //pointers to data inputs and data output
    CIEC_ANY *pDataInput;
    CIEC_ANY *pDataOutput = getDO(0);

    int nUsedBytes = -1;
    TForteUInt16 nCurrentLength = 0;
    TForteUInt16 nArrayElements = 0;
    TForteUInt16 nRequiredBytes = 0;
    TForteUInt16 nStringLength = 0;

    //iterate over data inputs
    for(size_t input_index = 0; input_index < mDInputs; input_index++) {
      //get current data input
      pDataInput = getDI(static_cast<unsigned int>(input_index));

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
          TForteUInt16 nBytesFree = static_cast<TForteUInt16>((static_cast<CIEC_STRING*>(pDataOutput))->getCapacity() - nCurrentLength);

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

bool GEN_APPEND_STRING::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec){
  const char *pcPos = strrchr(paConfigString, '_');

  if(0 != pcPos){
    ++pcPos;
    if('S' != *pcPos){
      //we have an underscore and it is not the first underscore after E
      mDInputs = static_cast<size_t>(forte::core::util::strtoul(pcPos, 0, 10));
    }
    else{
      mDInputs = 0;
    }
  }
  else{
    return false;
  }

  if(mDInputs < 2){
    return false;
  }

  //now the number of needed eventInputs and dataOutputs are available in the integer array
  //create the eventInputs
  if(mDInputs < CFunctionBlock::scm_nMaxInterfaceEvents){

    //create the data inputs
    m_anDataInputNames = new CStringDictionary::TStringId[mDInputs];
    m_anDataInputTypeIds = new CStringDictionary::TStringId[mDInputs];

    generateGenericDataPointArrays("IN_", m_anDataInputTypeIds, m_anDataInputNames, mDInputs);

    //now create the WITH constructs...
    //the With-Indexes for the data variables
    m_anEIWith = new TDataIOID[mDInputs + 1]; //for inputs + '255' separators at the list end

    //in-withs
    for(size_t in_with = 0; in_with < mDInputs; ++in_with){
      m_anEIWith[in_with] = static_cast<TDataIOID>(in_with);
    }
    m_anEIWith[mDInputs] = scmWithListDelimiter;

    //create the interface Specification
    paInterfaceSpec.m_nNumEIs = 1;
    paInterfaceSpec.m_aunEINames = scm_anEventInputNames;
    paInterfaceSpec.m_anEIWith = m_anEIWith;
    paInterfaceSpec.m_anEIWithIndexes = scm_anEIWithIndexes;
    paInterfaceSpec.m_nNumEOs = 1;
    paInterfaceSpec.m_aunEONames = scm_anEventOutputNames;
    paInterfaceSpec.m_anEOWith = scm_anEOWith;
    paInterfaceSpec.m_anEOWithIndexes = scm_anEOWithIndexes;
    paInterfaceSpec.m_nNumDIs = static_cast<TForteUInt8>(mDInputs);
    paInterfaceSpec.m_aunDINames = m_anDataInputNames;
    paInterfaceSpec.m_aunDIDataTypeNames = m_anDataInputTypeIds;
    paInterfaceSpec.m_nNumDOs = 1;
    paInterfaceSpec.m_aunDONames = scm_anDataOutputNames;
    paInterfaceSpec.m_aunDODataTypeNames = scm_anDataOutputTypeIds;
    return true;
  }
  return false;
}
