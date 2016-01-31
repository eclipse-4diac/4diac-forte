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

#ifndef _PROCESSINTERFACE_H_
#define _PROCESSINTERFACE_H_

#include <../../stdfblib/io/processinterfacebase.h>
#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_string.h>

class CProcessInterface : public CProcessInterfaceBase{

public:
  CProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
                    const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData);
  virtual ~CProcessInterface();

protected:
  bool initialise(bool paInput);
  bool deinitialise();
  bool writePin();
  bool readPin();
  bool writeWord();
  bool readWord();

private:
  const static int scmBuffer = 100;

  int mFd;
  int mDeviceAddress;
  int mValueAddress;

  static const char * const scmInitDeinitOK;
  static const char * const scmNotInitialised;
};

#endif /* _PROCESSINTERFACE_H_ */
