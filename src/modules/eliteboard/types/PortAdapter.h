/*******************************************************************************
 * Copyright (c) 2021, 2022 Jonathan Lainer (kontakt@lainer.co.at)
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *******************************************************************************/

#ifndef _PORTADAPTER_H_
#define _PORTADAPTER_H_

#include "adapter.h"
#include "forte_array_at.h"
#include "forte_dword.h"
#include "typelib.h"

class FORTE_PortAdapter : public CAdapter {
  DECLARE_ADAPTER_TYPE(FORTE_PortAdapter)

private:
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];

public:
  static const TEventID scm_nEventMAPOID = 0;

private:
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

public:
  static const TEventID scm_nEventMAPID = 0;

private:
  static const TDataIOID scm_anEOWith[];
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpecSocket;

  static const SFBInterfaceSpec scm_stFBInterfaceSpecPlug;

public:
  TEventID MAPO() { return m_nParentAdapterListEventID + scm_nEventMAPOID; }

  TEventID MAP() { return m_nParentAdapterListEventID + scm_nEventMAPID; }

  CIEC_DWORD &GPIO_Port_Addr() {
    return *static_cast<CIEC_DWORD *>((isSocket()) ? getDO(0) : getDI(0));
  }

private:
  FORTE_ADAPTER_DATA_ARRAY(1, 1, 0, 1, 0);

public:
  FORTE_PortAdapter(CStringDictionary::TStringId pa_anAdapterInstanceName,
                    CResource *pa_poSrcRes, bool pa_bIsPlug)
      : CAdapter(pa_poSrcRes, &scm_stFBInterfaceSpecSocket,
                 pa_anAdapterInstanceName, &scm_stFBInterfaceSpecPlug,
                 pa_bIsPlug, m_anFBConnData, m_anFBVarsData){};

  ~FORTE_PortAdapter() override = default;
};

#endif // _PORTADAPTER_H_
