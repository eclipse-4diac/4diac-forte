/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Waldemar Eisenmenger - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef _PROCESSINTERFACE_H_
#define _PROCESSINTERFACE_H_

#include <../../stdfblib/io/processinterfacebase.h>
#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_string.h>

class CSysFsProcessInterface : public CProcessInterfaceBase{

public:
 CSysFsProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
        const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData);
    virtual ~CSysFsProcessInterface();

protected:
  bool initialise(bool m_bInputOrOutput);
  bool deinitialise();
  bool writePin();
  bool readPin();

private:
  bool checkPin();
  const static int scmBuffer = 100;

  int mPinNumber;

  static const char * const scmPinInUse;
  static const char * const scmInitDeinitOK;
  static const char * const scmNotInitialised;

};

//tell the IX and QX FB that this is the process interface to be used
typedef CSysFsProcessInterface CProcessInterface;

#endif /* PROCESSINTERFACE_H_ */
