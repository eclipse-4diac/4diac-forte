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
  bool readPin();
  bool writePin();
  bool readWord();
  bool writeWord();
  bool readDWord();
  bool writeDWord();

  virtual void onHandle(IOHandle *handle);
  virtual void dropHandle();

private:
  bool isListening;
  bool isReady;
  bool isInput;

  static const char * const scmOK;
  static const char * const scmWaitingForHandle;
  static const char * const scmFailedToRegister;
  static const char * const scmMappedWrongDirectionOutput;
  static const char * const scmMappedWrongDirectionInput;
  static const char * const scmMappedWrongDataType;
};

}

typedef EmBrick::ProcessInterface CProcessInterface;

#endif
