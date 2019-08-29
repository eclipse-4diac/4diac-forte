/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH and Herwig Eichler, www.conmeleon.org
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl and Herwig Eichler
 *******************************************************************************/

#include "processinterface.h"

const char * const CConmeleonC1ProcessInterface::scmUnknownChannel = "Channel number not existing";
const char * const CConmeleonC1ProcessInterface::scmChannelNotSupported = "Channel type not supported by hardware";
const char * const CConmeleonC1ProcessInterface::scmChannelInUse = "Channel already in use by other FB";
const char * const CConmeleonC1ProcessInterface::scmInitDeinitOK = "OK";
const char * const CConmeleonC1ProcessInterface::scmNotInitialised = "Not initialized";
const char * const CConmeleonC1ProcessInterface::scmOK = "OK";

CONMELEON::CGpioPin CConmeleonC1ProcessInterface::smDigitalInputs[] = {
  {20, CONMELEON::input},
  {21, CONMELEON::input},
  {22, CONMELEON::input},
  {23, CONMELEON::input}
};

CONMELEON::CGpioPin CConmeleonC1ProcessInterface::smDigitalOutputs[] = {
  {24, CONMELEON::output},
  {25, CONMELEON::output},
  {26, CONMELEON::output},
  {27, CONMELEON::output}
};

CONMELEON::CAds1018 CConmeleonC1ProcessInterface::smADC("/dev/spidev0.1", 1000000, CONMELEON::SPIMODE1);

CConmeleonC1ProcessInterface::EIOState CConmeleonC1ProcessInterface::smAIUsage[5] = {
    CConmeleonC1ProcessInterface::enFree,
    CConmeleonC1ProcessInterface::enFree,
    CConmeleonC1ProcessInterface::enFree,
    CConmeleonC1ProcessInterface::enFree,
    CConmeleonC1ProcessInterface::enFree
};

CConmeleonC1ProcessInterface::EIOState CConmeleonC1ProcessInterface::smDIUsage[4] = {
    CConmeleonC1ProcessInterface::enFree,
    CConmeleonC1ProcessInterface::enFree,
    CConmeleonC1ProcessInterface::enFree,
    CConmeleonC1ProcessInterface::enFree
};

CConmeleonC1ProcessInterface::EIOState CConmeleonC1ProcessInterface::smDOUsage[4] = {
    CConmeleonC1ProcessInterface::enFree,
    CConmeleonC1ProcessInterface::enFree,
    CConmeleonC1ProcessInterface::enFree,
    CConmeleonC1ProcessInterface::enFree
};

CConmeleonC1ProcessInterface::CConmeleonC1ProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) :
    CProcessInterfaceBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData), mChannelNr(-1), mCallingFB(enUnsupported){

  // initialize the ADC settings
  smADC.setPGA(CONMELEON::FS4096);        // maximum input voltage of ADC is 3.3V so we use 4.096V as full scale setting
  smADC.setVoltageDivider(3.0);          // CONMELEON C1 input voltage divider has value of 3.0:  10.0V to 3.3V
  smADC.setConversionMode(CONMELEON::SingleShot); // set to single shot conversion mode

}

CConmeleonC1ProcessInterface::~CConmeleonC1ProcessInterface(){
}

bool CConmeleonC1ProcessInterface::initialise(bool m_bInputOrOutput){

  CIEC_INT param;
  param.fromString(PARAMS().getValue());
  mChannelNr = param;
  setCallingFBType(m_bInputOrOutput);

  if (!checkChannelBoundaries()){
    STATUS() = scmUnknownChannel;
    return false;
  }

  if (checkCallingFBTypeIsIW()){
    // we need to handle an analog input
    if (smAIUsage[mChannelNr-1] == enFree){
      smAIUsage[mChannelNr-1] = enUsed;
    }
    else{
      STATUS() = scmChannelInUse;
      return false;
    }
  }
  else if (checkCallingFBTypeIsIX()){
      // we need to handle an digital input
      if (smDIUsage[mChannelNr-1] == enFree){
        smDIUsage[mChannelNr-1] = enUsed;
       // conmeleon c1 digital inputs have inverted logic level
        smDigitalInputs[mChannelNr-1].setInverted(true);
      }
      else{
        STATUS() = scmChannelInUse;
        return false;
      }
  }
  else if (checkCallingFBTypeIsQX()){
    // we need to handle an digital output
    if (smDOUsage[mChannelNr-1] == enFree){
      smDOUsage[mChannelNr-1] = enUsed;
    }
    else{
      STATUS() = scmChannelInUse;
      return false;
    }
  }
  else {
    STATUS() = scmChannelNotSupported;
    return false;
  }
  STATUS() = scmInitDeinitOK;
  return true;
}

bool CConmeleonC1ProcessInterface::deinitialise(){

  if (checkChannelBoundaries()){

    if (checkCallingFBTypeIsIW()){
      smAIUsage[mChannelNr-1] = enFree;
    }
    else if (checkCallingFBTypeIsIX()){
      smDIUsage[mChannelNr-1] = enFree;
    }
    else if (checkCallingFBTypeIsQX()){
      smDOUsage[mChannelNr-1] = enFree;
    }
  }
  mChannelNr = -1;
  mCallingFB = enUnsupported;
  STATUS() = scmInitDeinitOK;
  return true;
}

bool CConmeleonC1ProcessInterface::readPin(){

  if (!checkChannelBoundaries()){
    STATUS() = scmUnknownChannel;
    return false;
  }
  if (checkCallingFBTypeIsIX()){
    IN_X() = smDigitalInputs[mChannelNr-1].read();
    STATUS() = scmOK;
    return true;
  }
  else {
    STATUS() = scmChannelNotSupported;
    return false;
  }
}

bool CConmeleonC1ProcessInterface::writePin(){

  if (!checkChannelBoundaries()){
    STATUS() = scmUnknownChannel;
    return false;
  }
  if (checkCallingFBTypeIsQX()){
    smDigitalOutputs[mChannelNr-1].write(OUT_X());
    STATUS() = scmOK;
    return true;
  }
  else {
    STATUS() = scmChannelNotSupported;
    return false;
  }
}

bool CConmeleonC1ProcessInterface::readWord(){

  if (!checkChannelBoundaries()){
    STATUS() = scmUnknownChannel;
    return false;
  }

  if (checkCallingFBTypeIsIW()){
    if (mChannelNr <= 4) {
      // read voltage input
      IN_W() = static_cast<TForteWord>(smADC.readVoltage(mChannelNr-1));
    }
    if (mChannelNr == 5){
      // read temperature input
      IN_W() = static_cast<TForteWord>(smADC.readTemperature());
    }
    STATUS() = scmOK;
    return true;
  }
  else {
    STATUS() = scmChannelNotSupported;
    return false;
  }
}

void CConmeleonC1ProcessInterface::setCallingFBType(bool paIsInput){

  mCallingFB =  enUnsupported;

  if (paIsInput){
    // the only way is to check the interface of the FB
    if (getDO(2)->getDataTypeID() == CIEC_ANY::e_WORD){
      // IW FB called
      mCallingFB = enIW;
    }
    else if (getDO(2)->getDataTypeID() == CIEC_ANY::e_BOOL){
      // IX FB called
      mCallingFB = enIX;
    }
  }
  else {
    if (getDI(2)->getDataTypeID() == CIEC_ANY::e_BOOL){
      // QX FB called
      mCallingFB = enQX;
    }
  }
}

bool CConmeleonC1ProcessInterface::checkChannelBoundaries() const{
  // channel number should be between 1..4 for DI and DO and 1..5 for AI
  if ((mChannelNr > 0) && ((checkCallingFBTypeIsIW() && (mChannelNr <= 5)) || (mChannelNr <= 4))){
    return true;
  }
  return false;
}

