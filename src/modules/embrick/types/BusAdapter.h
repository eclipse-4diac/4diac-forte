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

#ifndef SRC_MODULES_EMBRICK_TYPES_BUSADAPTER_H_
#define SRC_MODULES_EMBRICK_TYPES_BUSADAPTER_H_

#include <adapter.h>
#include <typelib.h>
#include <forte_int.h>

namespace EmBrick {
namespace FunctionBlocks {

class BusAdapter: public CAdapter {
DECLARE_ADAPTER_TYPE(BusAdapter)

private:
private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
public:
  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>((isSocket()) ? getDI(0) : getDO(0));
  }

private:
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
public:
  CIEC_INT &INDEX() {
    return *static_cast<CIEC_INT*>((isSocket()) ? getDO(0) : getDI(0));
  }

public:
  static const TEventID scm_nEventINITOID = 0;
  int INITO() {
    return m_nParentAdapterListEventID + scm_nEventINITOID;
  }
private:
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

public:
  static const TEventID scm_nEventINITID = 0;
  int INIT() {
    return m_nParentAdapterListEventID + scm_nEventINITID;
  }
private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpecSocket;

  static const SFBInterfaceSpec scm_stFBInterfaceSpecPlug;

  FORTE_ADAPTER_DATA_ARRAY(1, 1, 1, 1, 0)

public:
  ADAPTER_CTOR(BusAdapter){};

  virtual ~BusAdapter() {};

};

}
/* namespace FunctionsBlocks */
} /* namespace EmBrick */

#endif //close the ifdef sequence from the beginning of the file

