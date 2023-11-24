/*******************************************************************************
 * Copyright (c) 2023 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Mario Kastner, Alois Zoitl - initial implementation
 *******************************************************************************/

#include "commfb.h"
#include "parameterParser.h"
#include "structmembercomlayer.h"
#include "typelib.h"

using namespace forte::com_infra;

CStructMemberLocalComLayer::CStructMemberLocalComLayer(CComLayer* paUpperLayer, CBaseCommFB * paFB) :
    CLocalComLayer(paUpperLayer, paFB){
}

void CStructMemberLocalComLayer::setRDs(forte::com_infra::CBaseCommFB &paSubl, CIEC_ANY **paSDs, TPortId ){
  CIEC_ANY* target = getTargetByIndex(static_cast<CIEC_STRUCT*>(&(paSubl.getRDs()[0]->unwrap())), mIndexList);
  if (nullptr != target)
	  target->setValue(paSDs[0]->unwrap());
}

CIEC_ANY* CStructMemberLocalComLayer::getTargetByIndex(CIEC_STRUCT* paRoot, TTargetStructIndexList &paIndexList){
	if (paIndexList.empty()) {
		return nullptr;
	}

	CIEC_STRUCT* target = paRoot;
	for (auto index : paIndexList) {
		target = static_cast<CIEC_STRUCT*>(target->getMember(index));
	}
	return target;
}

CStructMemberLocalComLayer::TTargetStructIndexList CStructMemberLocalComLayer::buildIndexList(CIEC_ANY* paRoot, const char *paNestedStructString) {
  CParameterParser parser(paNestedStructString, '.');
  const size_t numNestedStructs = parser.parseParameters();
  TTargetStructIndexList resultList;

  for (size_t i = 0; i < numNestedStructs; i++) {
  	CStringDictionary::TStringId id = CStringDictionary::getInstance().insert(parser[i]);
  	size_t memberIndex = static_cast<CIEC_STRUCT*>(paRoot)->getMemberIndex(id);

  	if (memberIndex == CIEC_STRUCT::csmNIndex) {
  		resultList.clear(); //on error return empty resultList
  		return resultList;
  	}

  	resultList.push_back(memberIndex);
  	CIEC_ANY* member = static_cast<CIEC_STRUCT*>(paRoot)->getMember(memberIndex);

  	if (member->unwrap().getDataTypeID() == CIEC_ANY::e_STRUCT) {
  		paRoot = member;
	  }
  }
  return resultList;
}

EComResponse CStructMemberLocalComLayer::openConnection(char *paLayerParameter){
  //structmemb[localgroupname;structtype;structmembername.structchildmembername]
  CParameterParser parser(paLayerParameter, ';', scmNumLayerParameters);

  if (parser.parseParameters() != scmNumLayerParameters) {
    DEVLOG_ERROR("[StructMemberLayer] The initialization string might be wrong! Usage: structmemb[<localGroupName>;<StructDataTypeName>;<StructMemberName>.<StructChildMembername>]");
    return e_InitInvalidId;
  }

  if ((nullptr != mFb) && (mFb->getNumSD() > 1 || mFb->getNumRD() > 1)) {
    DEVLOG_ERROR("[StructMemberLayer] Only FBs with single data ports are supported!");
    return e_InitInvalidId;
  }

  CStringDictionary::TStringId groupNameID = CStringDictionary::getInstance().insert(parser[EComStringIndex::e_LOCALGROUPNAME]);
  CStringDictionary::TStringId dataTypeNameID = CStringDictionary::getInstance().insert(parser[EComStringIndex::e_STRUCTTYPE]);

  CIEC_STRUCT *const dummy = static_cast<CIEC_STRUCT* >(CTypeLib::createDataTypeInstance(dataTypeNameID, nullptr));

  if(nullptr == dummy){
    DEVLOG_ERROR("[StructMemberLayer] The struct is not available in the data type lib: %s!\r\n", parser[EComStringIndex::e_STRUCTTYPE]);
    return e_InitInvalidId;
  }

  mIndexList = buildIndexList(dummy, parser[EComStringIndex::e_STRUCTMEMBERNAME]);

  if (mIndexList.empty()) {
    DEVLOG_ERROR("[StructMemberLayer] The specified struct has no member \"%s\"!\r\n", parser[EComStringIndex::e_STRUCTMEMBERNAME]);
    return e_InitInvalidId;
  }

  switch (mFb->getComServiceType()){
    case e_Server:
    case e_Client:
      break;
    case e_Publisher: {
      CIEC_ANY *dummySDs[] = {dummy};
      mLocalCommGroup = getLocalCommGroupsManager().registerPubl(groupNameID, this, dummySDs, 1);
      }
      break;
    case e_Subscriber:
      break;
  }

  return (nullptr != mLocalCommGroup) ? e_InitOk : e_InitInvalidId;
}
