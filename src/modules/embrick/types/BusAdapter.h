/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#ifndef SRC_MODULES_EMBRICK_TYPES_BUSCONFIGADAPTER_H_
#define SRC_MODULES_EMBRICK_TYPES_BUSCONFIGADAPTER_H_

#include <io/configFB/io_adapter_multi.h>
#include <typelib.h>
#include <forte_bool.h>
#include <forte_uint.h>

class EmbrickBusAdapter: public forte::core::io::IOConfigFBMultiAdapter {
DECLARE_ADAPTER_TYPE(EmbrickBusAdapter)

private:
private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
public:

  CIEC_UINT &UpdateInterval() {
    return *static_cast<CIEC_UINT*>((isSocket()) ? getDO(3) : getDI(3));
  }

private:
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpecSocket;

  static const SFBInterfaceSpec scm_stFBInterfaceSpecPlug;

  FORTE_ADAPTER_DATA_ARRAY(1, 1, 1, 4, 0)

public:
  ADAPTER_CTOR_FOR_IO_MULTI(EmbrickBusAdapter){
};

private:
  static const TForteUInt8 scmSlaveConfigurationIO[];
  static const TForteUInt8 scmSlaveConfigurationIONum;

  virtual ~EmbrickBusAdapter() {};

};

#endif //close the ifdef sequence from the beginning of the file

