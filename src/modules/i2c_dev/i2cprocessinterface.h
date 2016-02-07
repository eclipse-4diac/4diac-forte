/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Gerd Kainz, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef _I2CPROCESSINTERFACE_H_
#define _I2CPROCESSINTERFACE_H_

#include <../../stdfblib/io/processinterfacebase.h>
#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_string.h>
#include <string>
#include <vector>

class CI2CProcessInterface : public CProcessInterfaceBase{

  public:
    CI2CProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
        const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData,
        TForteByte *paFBVarsData);
    virtual ~CI2CProcessInterface();

  protected:
    bool initialise(bool paInput);
    bool deinitialise();
    bool writePin();
    bool readPin();
    bool writeWord();
    bool readWord();

  protected:
    const static int scmBuffer = 100;

    int mFd;
    int mDeviceAddress;
    int mValueAddress;

  private:
    static const char * const scmOK;
    static const char * const scmInvalidParam;
    static const char * const scmNotInitialised;
    static const char * const scmCouldNotRead;
    static const char * const scmCouldNotWrite;

    //! ioctl command id for setting the slave id to be used for the next reads and writes
    static const int scmSetSlaveId = 0x0703;

    std::vector<std::string> generateParameterList();
};

#endif /* _I2CPROCESSINTERFACE_H_ */
