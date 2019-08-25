/*******************************************************************************
 * Copyright (c) 2015, 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Gerd Kainz, Alois Zoitl - initial API and implementation and/or initial documentation
 *    Alois Zoitl - changed from i2c-dev to simple read writes
 *******************************************************************************/

#ifndef _I2CPROCESSINTERFACE_H_
#define _I2CPROCESSINTERFACE_H_

#include <../../stdfblib/io/processinterfacebase.h>
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
    TForteByte mValueAddress;

    static const char * const scmOK;
    static const char * const scmInvalidParam;
    static const char * const scmNotInitialised;
    static const char * const scmCouldNotRead;
    static const char * const scmCouldNotWrite;

  private:
    //! ioctl command id for setting the slave id to be used for the next reads and writes
    static const int scmSetSlaveId = 0x0703;

    std::vector<std::string> generateParameterList();
};

#endif /* _I2CPROCESSINTERFACE_H_ */
