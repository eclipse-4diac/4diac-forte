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

#ifndef _PORT_H_
#define _PORT_H_

#include <array>
#include "PortAdapter.h"
#include "forte_array_at.h"
#include "forte_wstring.h"
#include "funcbloc.h"

class FORTE_Port : public CFunctionBlock {
  DECLARE_FIRMWARE_FB(FORTE_Port)

private:
  static const CStringDictionary::TStringId scmDataInputNames[];
  static const CStringDictionary::TStringId scmDataInputTypeIds[];

  static const int scmPortInAdapterAdpNum = 0;

  static const SAdapterInstanceDef scmAdapterInstances[];

  static const SFBInterfaceSpec scmFBInterfaceSpec;

  CIEC_WSTRING &st_Pin0() { return *static_cast<CIEC_WSTRING *>(getDI(0)); }

  CIEC_WSTRING &st_Pin1() { return *static_cast<CIEC_WSTRING *>(getDI(1)); }

  CIEC_WSTRING &st_Pin2() { return *static_cast<CIEC_WSTRING *>(getDI(2)); }

  CIEC_WSTRING &st_Pin3() { return *static_cast<CIEC_WSTRING *>(getDI(3)); }

  CIEC_WSTRING &st_Pin4() { return *static_cast<CIEC_WSTRING *>(getDI(4)); }

  CIEC_WSTRING &st_Pin5() { return *static_cast<CIEC_WSTRING *>(getDI(5)); }

  CIEC_WSTRING &st_Pin6() { return *static_cast<CIEC_WSTRING *>(getDI(6)); }

  CIEC_WSTRING &st_Pin7() { return *static_cast<CIEC_WSTRING *>(getDI(7)); }

  CIEC_WSTRING &st_Pin8() { return *static_cast<CIEC_WSTRING *>(getDI(8)); }

  CIEC_WSTRING &st_Pin9() { return *static_cast<CIEC_WSTRING *>(getDI(9)); }

  CIEC_WSTRING &st_Pin10() { return *static_cast<CIEC_WSTRING *>(getDI(10)); }

  CIEC_WSTRING &st_Pin11() { return *static_cast<CIEC_WSTRING *>(getDI(11)); }

  CIEC_WSTRING &st_Pin12() { return *static_cast<CIEC_WSTRING *>(getDI(12)); }

  CIEC_WSTRING &st_Pin13() { return *static_cast<CIEC_WSTRING *>(getDI(13)); }

  CIEC_WSTRING &st_Pin14() { return *static_cast<CIEC_WSTRING *>(getDI(14)); }

  CIEC_WSTRING &st_Pin15() { return *static_cast<CIEC_WSTRING *>(getDI(15)); }

  FORTE_PortAdapter &st_PortInAdapter() {
    return (*static_cast<FORTE_PortAdapter *>(mAdapters[0]));
  };


  // The (maximum) number  of IO pins available on a GPIO port.
  static constexpr size_t pin_cnt = 16;
  std::array<CIEC_WSTRING *, pin_cnt> mRegistered;

  void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;
  void deregister_handles();
  void register_handles();


public:
  FORTE_Port(const CStringDictionary::TStringId paInstanceNameId,
             CResource *paSrcRes)
      : CFunctionBlock(paSrcRes, &scmFBInterfaceSpec, paInstanceNameId),
        mRegistered{} {};

  ~FORTE_Port() override = default;
};

#endif // _PORT_H_
