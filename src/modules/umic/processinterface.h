/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef _UMICPROCINT_H_
#define _UMICPROCINT_H_

#include <../../stdfblib/io/processinterfacebase.h>
#include <forte_sync.h>
#include <string>


#define UMIC_LED_ENABLED 1
#define UMIC_RELAY_ENABLED 1

enum UMIC_IO_TYPE{
  UNDEFINED,
  DIO,
#if UMIC_LED_ENABLED
  LED,
#endif
#if UMIC_RELAY_ENABLED
  RELAY,
#endif
};

class CUMICProcessInterface : public CProcessInterfaceBase{

  public:
    CUMICProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData);
    virtual ~CUMICProcessInterface();

  protected:
    bool initialise(bool paIsInput);
    bool deinitialise();
    bool writePin();
    bool readPin();

    UMIC_IO_TYPE mIOType;
    uint8_t mNumber;
    static bool smDIOInitialized;

    static uint8_t smIODirections;

    static const std::string scmDIOID;
#if UMIC_LED_ENABLED
    static const std::string scmLEDID;
#endif
#if UMIC_RELAY_ENABLED
    static const std::string scmRELAYID;
#endif
    static CSyncObject directionsMutex;

    static const char * const scmOK;
    static const char * const scmPinInUse;
    static const char * const scmNotInitialised;
    static const char * const scmError;
    static const char * const scmCouldNotRead;
    static const char * const scmCouldNotWrite;

};

typedef CUMICProcessInterface CProcessInterface;

#endif /* PROCESSINTERFACE_H_ */



