/*******************************************************************************
 * Copyright (c) 2012 - 2014 AIT, ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "EplXmlReader.h"

#include <string_utils.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

#include <tinyxml.h>

CEplXmlReader::CEplXmlReader(CProcessImageMatrix* paIn, CProcessImageMatrix* paOut){
  mProcImageIn = paIn;
  mProcImageOut = paOut;
}

CEplXmlReader::~CEplXmlReader(){

}

void CEplXmlReader::setProcessImageIn(CProcessImageMatrix* paIn){
  mProcImageIn = paIn;
}

void CEplXmlReader::setProcessImageOut(CProcessImageMatrix* paOut){
  mProcImageOut = paOut;
}

void CEplXmlReader::readXmlFile(const char* paFileName){

  TiXmlDocument xmlDoc(paFileName);

  if(xmlDoc.LoadFile()){
    TiXmlNode *appProcess = xmlDoc.FirstChild("ApplicationProcess");
    TiXmlNode *processImageIn;
    for(TiXmlNode *processImage = appProcess->ToElement()->FirstChild("ProcessImage"); processImage != nullptr; processImage = processImage->NextSibling("ProcessImage")){
      if(strcmp(processImage->ToElement()->Attribute("type"), "output") == 0){
        createProcImageOut(processImage);
      }
      else if(strcmp(processImage->ToElement()->Attribute("type"), "input") == 0){
        processImageIn = processImage;
      }
    }
    createProcImageIn(processImageIn);
  }
  else{
    cout << "ERROR: Could not open XML file" << endl;
  }
}

void CEplXmlReader::createProcImageOut(TiXmlNode* paProcessImage){
  int currentCnId = -1;
  int currentModuleNr = -1;
  char currentModuleId[256];
  int currentIoNr = -1;

  TiXmlNode *channel;
  for(channel = paProcessImage->FirstChild("Channel"); channel != nullptr; channel = channel->NextSibling("Channel")){
    // New IO
    currentIoNr++;

    // Get "Name" attribute
    const char *ioName = channel->ToElement()->Attribute("Name");
    char *nameStr = new char[strlen(ioName) + 1];
    strcpy(nameStr, ioName);

    // Get CN ID
    char* pch = strtok(nameStr, ".CN");
    int cnId = forte::core::util::strtol(pch,0,10);

    // Get module ID
    char* modId = strtok(nullptr, ".");

    // Get dataSize
    const char *temp = channel->ToElement()->Attribute("dataSize");
    int dSize = forte::core::util::strtoul(temp,0,10);

    // Get PIOffset
    temp = channel->ToElement()->Attribute("PIOffset");
    long piOffset = forte::core::util::strtol(temp, nullptr, 16);

    // Get BitOffset
    long bitOffset = 0;
    temp = channel->ToElement()->Attribute("BitOffset");
    if(temp != nullptr){
      bitOffset = forte::core::util::strtol(temp, nullptr, 16);
    }

    // Check CN id, module number and add new IO
    if(cnId != currentCnId){ // New CN
      currentCnId = cnId;
      currentModuleNr = 0;
      strcpy(currentModuleId, modId);
      currentIoNr = 0;

      mModuleListOut.addEntry(ioName, piOffset, bitOffset, currentModuleNr);
      mProcImageOut->addEntry(cnId, currentModuleNr, currentIoNr, dSize, piOffset, bitOffset);

    }
    else if(mModuleListOut.moduleNameExist(ioName)){ // IO name exist => this must be a new module with same brand as another
      currentModuleNr++;
      strcpy(currentModuleId, modId);
      currentIoNr = 0;

      mModuleListOut.addEntry(ioName, piOffset, bitOffset, currentModuleNr);
      mProcImageOut->addEntry(cnId, currentModuleNr, currentIoNr, dSize, piOffset, bitOffset);

    }
    else if(strcmp(modId, currentModuleId) == 0){ // same module ID as IO before => must be same module

      mProcImageOut->addEntry(cnId, currentModuleNr, currentIoNr, dSize, piOffset, bitOffset);

    }
    else{ // new module ID => new module
      currentModuleNr++;
      strcpy(currentModuleId, modId);
      currentIoNr = 0;

      mModuleListOut.addEntry(ioName, piOffset, bitOffset, currentModuleNr);
      mProcImageOut->addEntry(cnId, currentModuleNr, currentIoNr, dSize, piOffset, bitOffset);

    }

    cout << "<< " << currentCnId << ", " << currentModuleNr << ", " << ioName << ", " << currentIoNr << ", " << dSize << ", " << piOffset << ", " << bitOffset << endl;

    delete[] nameStr;
  }
}

void CEplXmlReader::createProcImageIn(TiXmlNode *paProcessImage){
  int currentCnId = -1;
  int currentModuleNr = -1;
  char currentModuleId[256];
  int currentIoNr = -1;

  TiXmlNode *channel;
  for(channel = paProcessImage->FirstChild("Channel"); channel != nullptr; channel = channel->NextSibling("Channel")){
    // New IO
    currentIoNr++;

    // Get "Name" attribute
    const char *ioName = channel->ToElement()->Attribute("Name");
    char *nameStr = new char[strlen(ioName) + 1];
    strcpy(nameStr, ioName);

    // Get CN ID
    char* pch = strtok(nameStr, ".CN");
    int cnId = forte::core::util::strtoul(pch,0,10);

    // Get module ID
    char* modId = strtok(nullptr, ".");

    // Get dataSize
    const char *temp = channel->ToElement()->Attribute("dataSize");
    int dSize = forte::core::util::strtoul(temp,0,10);

    // Get PIOffset
    temp = channel->ToElement()->Attribute("PIOffset");
    long piOffset = forte::core::util::strtol(temp, nullptr, 16);

    // Get BitOffset
    long bitOffset = 0;
    temp = channel->ToElement()->Attribute("BitOffset");
    if(temp != nullptr){
      bitOffset = forte::core::util::strtol(temp, nullptr, 16);
    }

    // Check CN id, module number and add new IO
    if(cnId != currentCnId){ // New CN
      currentCnId = cnId;

      currentModuleNr = getModuleNr(ioName);

      strcpy(currentModuleId, modId);
      currentIoNr = 0;

      mModuleListIn.addEntry(ioName, piOffset, bitOffset, currentModuleNr);
      mProcImageIn->addEntry(cnId, currentModuleNr, currentIoNr, dSize, piOffset, bitOffset);

    }
    else if(mModuleListIn.moduleNameExist(ioName)){ // IO name exist => this must be a new module with same brand as another

      currentModuleNr = getModuleNr(ioName);

      strcpy(currentModuleId, modId);
      currentIoNr = 0;

      mModuleListIn.addEntry(ioName, piOffset, bitOffset, currentModuleNr);
      mProcImageIn->addEntry(cnId, currentModuleNr, currentIoNr, dSize, piOffset, bitOffset);

    }
    else if(strcmp(modId, currentModuleId) == 0){ // same module ID as IO before => must be same module

      mProcImageIn->addEntry(cnId, currentModuleNr, currentIoNr, dSize, piOffset, bitOffset);

    }
    else{ // new module ID => new module

      currentModuleNr = getModuleNr(ioName);

      strcpy(currentModuleId, modId);
      currentIoNr = 0;

      mModuleListIn.addEntry(ioName, piOffset, bitOffset, currentModuleNr);
      mProcImageIn->addEntry(cnId, currentModuleNr, currentIoNr, dSize, piOffset, bitOffset);

    }

    cout << ">> " << currentCnId << ", " << currentModuleNr << ", " << ioName << ", " << currentIoNr << ", " << dSize << ", " << piOffset << ", " << bitOffset << endl;

    delete[] nameStr;
  }
}

int CEplXmlReader::getModuleNr(const char* paIoId){
  char localCopy[256];
  strcpy(localCopy, paIoId);

  char dest[256];
  char* pch = strtok(localCopy, ".");
  strcpy(dest, pch);
  strcat(dest, ".");
  pch = strtok(nullptr, ".");
  strcat(dest, pch);

  int occurences = mModuleListIn.getNrOfModules(dest);
  int modNr = mModuleListOut.getModuleNr(dest, occurences + 1);
  if(modNr == -1) {
    cout << "ShouldNotHappenError" << endl;
  }

  return modNr;
}
