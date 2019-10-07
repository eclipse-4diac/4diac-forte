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

#ifndef _PROCESSINTERFACE_H_
#define _PROCESSINTERFACE_H_

#include <../../stdfblib/io/processinterfacebase.h>
#include <funcbloc.h>
#include "gpio/gpiopin.h"
#include "spi/ads1018.h"

class CConmeleonC1ProcessInterface : public CProcessInterfaceBase{

public:
 CConmeleonC1ProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
        const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData);
    virtual ~CConmeleonC1ProcessInterface();

protected:
  bool initialise(bool m_bInputOrOutput);
  bool deinitialise();
  bool writePin();
  bool readPin();
  bool readWord();

private:

  int mChannelNr;
  static CONMELEON::CGpioPin smDigitalInputs[4];
  static CONMELEON::CGpioPin smDigitalOutputs[4];

  static CONMELEON::CAds1018 smADC;

  static const char * const scmUnknownChannel;
  static const char * const scmChannelNotSupported;
  static const char * const scmChannelInUse;
  static const char * const scmInitDeinitOK;
  static const char * const scmNotInitialised;
  static const char * const scmOK;

  enum EIOState{
      enFree,
      enUsed
  };

  enum ECallingFBType{
      enUnsupported,
      enIX,
      enIW,
      enQX
    };
  // we need this to check, which FB (IX, QX, or IW) is using this instance of CProcessInterface
  ECallingFBType mCallingFB;

  // CONMELEON C1 has 4 voltage inputs and an internal temperature readout, 4 digital inputs and 4 digital outputs
  static EIOState smAIUsage[5];
  static EIOState smDIUsage[4];
  static EIOState smDOUsage[4];

  void setCallingFBType(bool paIsInput);
  bool checkCallingFBTypeIsIW() const { return mCallingFB == enIW; };
  bool checkCallingFBTypeIsIX() const { return mCallingFB == enIX; };
  bool checkCallingFBTypeIsQX() const { return mCallingFB == enQX; };
  bool checkChannelBoundaries() const;

};

//tell the IX and QX FB that this is the process interface to be used
typedef CConmeleonC1ProcessInterface CProcessInterface;

#endif /* PROCESSINTERFACE_H_ */
