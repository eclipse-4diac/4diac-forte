/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef _PROCESSINTERFACE_H_
#define _PROCESSINTERFACE_H_

#include <../../stdfblib/io/processinterfacebase.h>
#include <vector>
#include <string>
#include <fstream>

class CLMSEV3ProcessInterface : public CProcessInterfaceBase{

  public:
    CLMSEV3ProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData);
    virtual ~CLMSEV3ProcessInterface();

  protected:
    bool initialise(bool paInput);
    bool deinitialise();
    bool writePin();
    bool readPin();

  private:

    static const std::string scmLEDId;
    static const std::string scmSensorID;
    static const std::string scmButtonID;

    static std::string getSensorBasePath(const std::string &paParam);

    std::vector<std::string> generateParameterList();

    bool setupLEDOutput(const std::vector<std::string> &paParamList);
    bool setupSensor(const std::string &paParam);
    bool setupButton(const std::vector<std::string> &paParamList);
    static int findNumberFromPort(const std::string &paBasePath, const std::string &paEv3Port);


    std::fstream mFile; //!< the file to be used for this process interface instance
    int typeOfInput;
    int button;
    int fileDescriptor;
};

//tell the IX and QX FB that this is the process interface to be used
typedef CLMSEV3ProcessInterface CProcessInterface;

#endif /* PROCESSINTERFACE_H_ */
