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
  CIEC_STRUCT& target = static_cast<CIEC_STRUCT &>(paSubl.getRDs()[0]->unwrap());
  target.getMember(mMemberIndex)->setValue(paSDs[0]->unwrap());
}

EComResponse CStructMemberLocalComLayer::openConnection(char *paLayerParameter){
  //structmemb[localgroupname;structtype;structmembername]
  CParameterParser parser(paLayerParameter, ';', scmNumLayerParameters);

  if (parser.parseParameters() != scmNumLayerParameters) {
    DEVLOG_ERROR("[StructMemberLayer] The initialization string might be wrong! Usage: structmemb[<localGroupName>;<StructDataTypeName>;<StructMemberName>]");
    return e_InitInvalidId;
  }

  if ((nullptr != mFb) && (mFb->getNumSD() > 1 || mFb->getNumRD() > 1)) {
    DEVLOG_ERROR("[StructMemberLayer] Only FBs with single data ports are supported!");
    return e_InitInvalidId;
  }

  CStringDictionary::TStringId groupNameID = CStringDictionary::getInstance().insert(parser[0]);
  CStringDictionary::TStringId dataTypeNameID = CStringDictionary::getInstance().insert(parser[1]);
  CStringDictionary::TStringId memberNameID = CStringDictionary::getInstance().insert(parser[2]);

  CIEC_STRUCT *const dummy = static_cast<CIEC_STRUCT*>(CTypeLib::createDataTypeInstance(dataTypeNameID, nullptr));

  if(dummy == nullptr){
    DEVLOG_ERROR("[StructMemberLayer] The struct is not available in the data type lib: %s!\r\n", parser[1]);
    return e_InitInvalidId;
  }

  //check if member name is present in struct
  mMemberIndex = dummy->getMemberIndex(memberNameID);
  if (mMemberIndex == CIEC_STRUCT::csmNIndex) {
    DEVLOG_ERROR("[StructMemberLayer] The specified struct has no member \"%s\"!\r\n", parser[2]);
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
