/*******************************************************************************
 * Copyright (c) 2016 -2019 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _PROCESSINTERFACE_H_
#define _PROCESSINTERFACE_H_

#include <../stdfblib/io/processinterfacebase.h>
#include "fmuValueContainer.h"
#include <vector>
#include <string>
#include <fstream>

#ifdef OUT
#undef OUT
#endif

#ifdef IN
#undef IN
#endif

class CFMUProcessInterface : public CProcessInterfaceBase{
  public:
    CFMUProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData);
    CFMUProcessInterface(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes);

    void setValueContainer(fmuValueContainer* paValueContainer);

    virtual ~CFMUProcessInterface();

    virtual void executeEvent(int ){};

  protected:
    bool initialise(bool paIsInput);
    bool deinitialise();
    bool writePin();
    bool readPin();
    bool readWord();
    bool writeWord();

    bool readDWord() {
      return false; //not implemented
    }
    bool writeDWord() {
      return false; //not implemented
    }

  private:
    static const char * const scmOK;
    static const char * const scmNOTINITIALIZED;
    static const char * const scmINTERNALERROR;

    bool mInitialized;

    fmuValueContainer* mValue;
};

//tell the IX and QX FB that this is the process interface to be used
typedef CFMUProcessInterface CProcessInterface;

#endif /* _PROCESSINTERFACE_H_ */
