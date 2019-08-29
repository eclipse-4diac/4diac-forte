/*******************************************************************************
 * Copyright (c) 2013 - 2015 Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Michael Hofmann, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _STEST_END_H_
#define _STEST_END_H_

#include <funcbloc.h>
#include <mgmcmdstruct.h>

class FORTE_STEST_END: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_STEST_END)

private:
  static const TEventID scm_nEventREQID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TForteInt16 scm_anEOWithIndexes[];
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

  FORTE_FB_DATA_ARRAY(0, 0, 0, 0);

  void executeEvent(int pa_nEIID);

public:
  FUNCTION_BLOCK_CTOR(FORTE_STEST_END){
  };

  virtual ~FORTE_STEST_END(){};

};

#endif //close the ifdef sequence from the beginning of the file

