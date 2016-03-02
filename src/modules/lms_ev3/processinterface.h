/*******************************************************************************
 * Copyright (c) 2015, 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Alois Zoitl - initial API and implementation and/or initial documentation
 *    Jose Cabral - expansion of capabilities
 *******************************************************************************/

#ifndef _PROCESSINTERFACE_H_
#define _PROCESSINTERFACE_H_

#include <../../stdfblib/io/processinterfacebase.h>
#include <vector>
#include <string>
#include <fstream>

struct st_ButtonVariables {
    int nmbutton;
    int nmFd;
};

enum ETypeOfIO {
  UNDEFINED,
  LED,
  SENSOR,
  SENSORW,
  BUTTON,
  MOTOR,
  PWM,
  SPEED
};

class CLMSEV3ProcessInterface : public CProcessInterfaceBase{

  public:
    CLMSEV3ProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData);
    virtual ~CLMSEV3ProcessInterface();

  protected:
    bool initialise(bool paInput);
    bool deinitialise();
    bool writePin();
    bool readPin();
    bool readWord();
    bool writeWord();

  private:

    static const std::string scmLEDID;
    static const std::string scmSensorID;
    static const std::string scmButtonID;
    static const std::string scmMotorID;
    static const std::string scmPWMID;
    static const std::string scmSensorWID;
    static const std::string scmSPEEDID;

    std::vector<std::string> generateParameterList();

    bool setupLED(const std::vector<std::string> &paParamList);
    bool setupSensor(const std::vector<std::string> &paParamList);
    bool setupSensorW(const std::vector<std::string> &paParamList);
    bool setupPWM(const std::string &paParam, bool paInput);
    bool setupMotor(const std::vector<std::string> &paParamList);
    bool setupButton(const std::string &paParam);
    bool setupSpeed(const std::string &paParam);
    bool readWordCore(TForteInt16* paResult);

    static int findNumberFromPort(const std::string &paBasePath, const std::string &paEv3Port);




    std::fstream mFile; //!< the file to be used for this process interface instance
    int mnTypeOfIO;
    int mnNoOfBits;
    struct st_ButtonVariables* mstButtonVariables;
    TForteWord mCountPerRot; //The speed is measured in tacho counts per second. This variable holds the information of how many tacho counts are in one rotation.
};

//tell the IX and QX FB that this is the process interface to be used
typedef CLMSEV3ProcessInterface CProcessInterface;

#endif /* PROCESSINTERFACE_H_ */