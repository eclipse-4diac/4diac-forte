/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef _QUERYSERVICESADP_H_
#define _QUERYSERVICESADP_H_

#include "forte/adapter.h"
#include "forte/typelib.h"
#include <forte_servicequeryform.h>
#include <forte_serviceregistryentry.h>
#include "forte/datatypes/forte_wstring.h"
#include "forte/datatypes/forte_array.h"

class FORTE_QueryServicesAdp : public CAdapter {
    DECLARE_ADAPTER_TYPE(FORTE_QueryServicesAdp)

  private:
  private:
  public:
    CIEC_ServiceRegistryEntry *serviceEntries() {
      return (CIEC_ServiceRegistryEntry *) (*static_cast<CIEC_ARRAY<> *>(
          (isSocket()) ? getDI(0) : getDO(0)))[0]; // the first element marks the start of the array
    };

    CIEC_ARRAY<> &serviceEntries_Array() {
      return *static_cast<CIEC_ARRAY<> *>((isSocket()) ? getDI(0) : getDO(0));
    };

  private:
  public:
    CIEC_ServiceQueryForm &serviceQueryForm() {
      return *static_cast<CIEC_ServiceQueryForm *>((isSocket()) ? getDO(0) : getDI(0));
    };

    CIEC_WSTRING &endpoint() {
      return *static_cast<CIEC_WSTRING *>((isSocket()) ? getDO(1) : getDI(1));
    };

  public:
    static const TEventID scmEventqueriedID = 0;
    TEventID queried() {
      return mParentAdapterListEventID + scmEventqueriedID;
    }

  private:
  public:
    static const TEventID scmEventqueryID = 0;
    TEventID query() {
      return mParentAdapterListEventID + scmEventqueryID;
    }

  private:
  public:
    ADAPTER_CTOR(FORTE_QueryServicesAdp) {};

    ~FORTE_QueryServicesAdp() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
