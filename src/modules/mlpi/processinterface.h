/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef PROCESSINTERFACE_H_
#define PROCESSINTERFACE_H_

#include <../../stdfblib/io/processinterfacebase.h>
#include <mlpiApiLib.h>

class CMLPIFaceProcessInterface : public CProcessInterfaceBase{
  public:
    CMLPIFaceProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
        const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData,
        TForteByte *paFBVarsData);

    virtual ~CMLPIFaceProcessInterface();

    static void connectToMLPI();

  protected:
    bool initialise(bool paInput);
    bool deinitialise();
    bool readPin();
    bool writePin();

    int mPin;

  private:
    WCHAR16* mVariableName;


    static MLPIHANDLE smConnection;
    static bool smInitialized;

};

//tell the IX and QX FB that this is the process interface to be used
typedef CMLPIFaceProcessInterface CProcessInterface;

#endif /* PROCESSINTERFACE_H_ */
