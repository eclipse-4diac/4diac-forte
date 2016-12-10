/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_MODULES_EMBRICK_PROCESSINTERFACE_H_
#define SRC_MODULES_EMBRICK_PROCESSINTERFACE_H_

#include <io/observer.h>
#include <slave/handle.h>
#include <../../stdfblib/io/processinterfacebase.h>

namespace EmBrick {

class ProcessInterface: public CProcessInterfaceBase, public IOObserver {

public:
  ProcessInterface(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
      const CStringDictionary::TStringId paInstanceNameId,
      TForteByte *paFBConnData, TForteByte *paFBVarsData);
  virtual ~ProcessInterface();

  bool onChange();

protected:
  bool initialise(bool paIsInput);
  bool deinitialise();
  bool writePin();
  bool readPin();

  virtual void onHandle(IOHandle *handle);
  virtual void dropHandle();

private:
  bool isReady;
  bool isInput;
};

}

typedef EmBrick::ProcessInterface CProcessInterface;

#endif
