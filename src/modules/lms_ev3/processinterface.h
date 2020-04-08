/*******************************************************************************
 * Copyright (c) 2015, 2016 fortiss GmbH
 *               2019 Robert Promok
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl - initial API and implementation and/or initial documentation
 *    Jose Cabral - expansion of capabilities
 *    Robert Promok - added functions
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
  SENSORW_VALUE,
  SENSORW_MODE,
  BUTTON,
  MOTOR_ENABLE,
  MOTOR_ENABLE_SPEED,
  MOTOR_RELATIVE_POSITION,
  MOTOR_ABSOLUTE_POSITION,
  MOTOR_RESET,
  MOTOR_PWM,
  MOTOR_SPEED,
  MOTOR_SPEEDSETPOINT,
  MOTOR_POSITIONSETPOINT,
  MOTOR_STOP,
  MOTOR_POSITION,
  MOTOR_ROT,
};

class CLMSEV3ProcessInterface : public CProcessInterfaceBase{

  public:
    CLMSEV3ProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData);
    virtual ~CLMSEV3ProcessInterface();

  protected:
    bool initialise(bool paIsInput);
    bool deinitialise();
    bool writePin();
    bool readPin();
    bool readWord();
    bool writeWord();
    bool readDWord();
    bool writeDWord();

  private:

    static const std::string scmLEDID;
    static const std::string scmSensorID;
    static const std::string scmSensorWID;
    static const std::string scmButtonID;
    static const std::string scmMotorID;

    static const std::string scmModeID; /* Used together wuth sensorw*/

    /* Used together with Motor */
    static const std::string scmEnableID;
    static const std::string scmEnableSpeedID;
    static const std::string scmRelPositionID;
    static const std::string scmAbsPositionID;

    static const std::string scmResetID;
    static const std::string scmPWMID;
    static const std::string scmSPEEDID;
    static const std::string scmSPEEDSETPOINTID;
    static const std::string scmPOSITIONSETPOINTID;
    static const std::string scmStopID;
    static const std::string scmPositionID;
    static const std::string scmRotID;

    static const char * const scmOK;
    static const char * const scmNotInitialised;
    static const char * const scmCouldNotRead;
    static const char * const scmCouldNotWrite;


    bool setupLED(const std::vector<std::string> &paParamList, bool paIsInput);
    bool setupSensor(const std::vector<std::string> &paParamList, bool paIsInput);
    bool setupSensorW(const std::vector<std::string> &paParamList, bool paIsInput);
    bool setupMotor(const std::vector<std::string> &paParamList, bool paIsInput);
    bool setupButton(const std::string &paParam, bool paIsInput);

    /**
     * return: 0 if no error, 1 if it is not initialized, 2 if it couldn't read
     */
    int readNumberFromFile(TForteInt32* paResult);

    bool setupSensorMode(const std::vector<std::string> &paParamList, bool paIsInput);
    bool setupSensorValue(const std::vector<std::string> &paParamList);


    std::vector<std::string> generateParameterList();
    static int findNumberFromPort(const std::string &paBasePath, const std::string &paEv3Port);

    std::fstream mFile; //!< the file to be used for this process interface instance
    int mnTypeOfIO;
    int mnNoOfBits;
    struct st_ButtonVariables* mstButtonVariables;
    std::vector<std::string> mModes;
};

//tell the IX and QX FB that this is the process interface to be used
typedef CLMSEV3ProcessInterface CProcessInterface;

#endif /* PROCESSINTERFACE_H_ */