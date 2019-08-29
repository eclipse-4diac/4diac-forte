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

CEplXmlReader::CEplXmlReader(CProcessImageMatrix* pa_pIn, CProcessImageMatrix* pa_pOut){
  m_pProcImageIn = pa_pIn;
  m_pProcImageOut = pa_pOut;
}

CEplXmlReader::~CEplXmlReader(){

}

void CEplXmlReader::setProcessImageIn(CProcessImageMatrix* pa_pIn){
  m_pProcImageIn = pa_pIn;
}

void CEplXmlReader::setProcessImageOut(CProcessImageMatrix* pa_pOut){
  m_pProcImageOut = pa_pOut;
}

void CEplXmlReader::readXmlFile(const char* pa_pchFileName){

  TiXmlDocument xmlDoc(pa_pchFileName);

  if(xmlDoc.LoadFile()){
    TiXmlNode *appProcess = xmlDoc.FirstChild("ApplicationProcess");
    TiXmlNode *processImageIn;
    for(TiXmlNode *processImage = appProcess->ToElement()->FirstChild("ProcessImage"); processImage != NULL; processImage = processImage->NextSibling("ProcessImage")){
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

void CEplXmlReader::createProcImageOut(TiXmlNode* pa_pProcessImage){
  int currentCnId = -1;
  int currentModuleNr = -1;
  char currentModuleId[256];
  int currentIoNr = -1;

  TiXmlNode *channel;
  for(channel = pa_pProcessImage->FirstChild("Channel"); channel != NULL; channel = channel->NextSibling("Channel")){
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
    char* modId = strtok(NULL, ".");

    // Get dataSize
    const char *temp = channel->ToElement()->Attribute("dataSize");
    int dSize = forte::core::util::strtoul(temp,0,10);

    // Get PIOffset
    temp = channel->ToElement()->Attribute("PIOffset");
    long piOffset = forte::core::util::strtol(temp, NULL, 16);

    // Get BitOffset
    long bitOffset = 0;
    temp = channel->ToElement()->Attribute("BitOffset");
    if(temp != NULL){
      bitOffset = forte::core::util::strtol(temp, NULL, 16);
    }

    // Check CN id, module number and add new IO
    if(cnId != currentCnId){ // New CN
      currentCnId = cnId;
      currentModuleNr = 0;
      strcpy(currentModuleId, modId);
      currentIoNr = 0;

      m_oModuleListOut.addEntry(ioName, piOffset, bitOffset, currentModuleNr);
      m_pProcImageOut->addEntry(cnId, currentModuleNr, currentIoNr, dSize, piOffset, bitOffset);

    }
    else if(m_oModuleListOut.moduleNameExist(ioName)){ // IO name exist => this must be a new module with same brand as another
      currentModuleNr++;
      strcpy(currentModuleId, modId);
      currentIoNr = 0;

      m_oModuleListOut.addEntry(ioName, piOffset, bitOffset, currentModuleNr);
      m_pProcImageOut->addEntry(cnId, currentModuleNr, currentIoNr, dSize, piOffset, bitOffset);

    }
    else if(strcmp(modId, currentModuleId) == 0){ // same module ID as IO before => must be same module

      m_pProcImageOut->addEntry(cnId, currentModuleNr, currentIoNr, dSize, piOffset, bitOffset);

    }
    else{ // new module ID => new module
      currentModuleNr++;
      strcpy(currentModuleId, modId);
      currentIoNr = 0;

      m_oModuleListOut.addEntry(ioName, piOffset, bitOffset, currentModuleNr);
      m_pProcImageOut->addEntry(cnId, currentModuleNr, currentIoNr, dSize, piOffset, bitOffset);

    }

    cout << "<< " << currentCnId << ", " << currentModuleNr << ", " << ioName << ", " << currentIoNr << ", " << dSize << ", " << piOffset << ", " << bitOffset << endl;

    delete[] nameStr;
  }
}

void CEplXmlReader::createProcImageIn(TiXmlNode *pa_pProcessImage){
  int currentCnId = -1;
  int currentModuleNr = -1;
  char currentModuleId[256];
  int currentIoNr = -1;

  TiXmlNode *channel;
  for(channel = pa_pProcessImage->FirstChild("Channel"); channel != NULL; channel = channel->NextSibling("Channel")){
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
    char* modId = strtok(NULL, ".");

    // Get dataSize
    const char *temp = channel->ToElement()->Attribute("dataSize");
    int dSize = forte::core::util::strtoul(temp,0,10);

    // Get PIOffset
    temp = channel->ToElement()->Attribute("PIOffset");
    long piOffset = forte::core::util::strtol(temp, NULL, 16);

    // Get BitOffset
    long bitOffset = 0;
    temp = channel->ToElement()->Attribute("BitOffset");
    if(temp != NULL){
      bitOffset = forte::core::util::strtol(temp, NULL, 16);
    }

    // Check CN id, module number and add new IO
    if(cnId != currentCnId){ // New CN
      currentCnId = cnId;

      currentModuleNr = getModuleNr(ioName);

      strcpy(currentModuleId, modId);
      currentIoNr = 0;

      m_oModuleListIn.addEntry(ioName, piOffset, bitOffset, currentModuleNr);
      m_pProcImageIn->addEntry(cnId, currentModuleNr, currentIoNr, dSize, piOffset, bitOffset);

    }
    else if(m_oModuleListIn.moduleNameExist(ioName)){ // IO name exist => this must be a new module with same brand as another

      currentModuleNr = getModuleNr(ioName);

      strcpy(currentModuleId, modId);
      currentIoNr = 0;

      m_oModuleListIn.addEntry(ioName, piOffset, bitOffset, currentModuleNr);
      m_pProcImageIn->addEntry(cnId, currentModuleNr, currentIoNr, dSize, piOffset, bitOffset);

    }
    else if(strcmp(modId, currentModuleId) == 0){ // same module ID as IO before => must be same module

      m_pProcImageIn->addEntry(cnId, currentModuleNr, currentIoNr, dSize, piOffset, bitOffset);

    }
    else{ // new module ID => new module

      currentModuleNr = getModuleNr(ioName);

      strcpy(currentModuleId, modId);
      currentIoNr = 0;

      m_oModuleListIn.addEntry(ioName, piOffset, bitOffset, currentModuleNr);
      m_pProcImageIn->addEntry(cnId, currentModuleNr, currentIoNr, dSize, piOffset, bitOffset);

    }

    cout << ">> " << currentCnId << ", " << currentModuleNr << ", " << ioName << ", " << currentIoNr << ", " << dSize << ", " << piOffset << ", " << bitOffset << endl;

    delete[] nameStr;
  }
}

int CEplXmlReader::getModuleNr(const char* pa_pchIoId){
  char localCopy[256];
  strcpy(localCopy, pa_pchIoId);

  char dest[256];
  char* pch = strtok(localCopy, ".");
  strcpy(dest, pch);
  strcat(dest, ".");
  pch = strtok(NULL, ".");
  strcat(dest, pch);

  int occurences = m_oModuleListIn.getNrOfModules(dest);
  int modNr = m_oModuleListOut.getModuleNr(dest, occurences + 1);
  if(modNr == -1) {
    cout << "ShouldNotHappenError" << endl;
  }

  return modNr;
}
