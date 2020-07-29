/*******************************************************************************
 * Copyright (c) 2020 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial tests
 *******************************************************************************/

#ifndef _STRUCT_MUX_DEMUX_DATA_H_
#define _STRUCT_MUX_DEMUX_DATA_H_

#include "forte_struct.h"

#include "forte_int.h"
#include "forte_string.h"


class CIEC_Struct_Muxer_Test_Struct_1 : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(Struct_Muxer_Test_Struct_1)

  public:
    CIEC_Struct_Muxer_Test_Struct_1();

    virtual ~CIEC_Struct_Muxer_Test_Struct_1() {
    }

    CIEC_INT &Var1() {
      return *static_cast<CIEC_INT*>(getMember(0));
    }

    CIEC_INT &Var2() {
      return *static_cast<CIEC_INT*>(getMember(1));
    }

    CIEC_STRING &Var3() {
      return *static_cast<CIEC_STRING*>(getMember(2));
    }

  private:
    static const CStringDictionary::TStringId scm_unElementTypes[];
    static const CStringDictionary::TStringId scm_unElementNames[];
};

class CIEC_Struct_Muxer_Test_Struct_2 : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(Struct_Muxer_Test_Struct_2)

  public:
    CIEC_Struct_Muxer_Test_Struct_2();

    virtual ~CIEC_Struct_Muxer_Test_Struct_2() {
    }

    CIEC_INT &Var1() {
      return *static_cast<CIEC_INT*>(getMember(0));
    }

    CIEC_STRING &Var2() {
      return *static_cast<CIEC_STRING*>(getMember(1));
    }

    CIEC_INT &Var3() {
      return *static_cast<CIEC_INT*>(getMember(2));
    }

  private:
    static const CStringDictionary::TStringId scm_unElementTypes[];
    static const CStringDictionary::TStringId scm_unElementNames[];
};

class CIEC_Struct_Muxer_Test_Struct_3 : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(Struct_Muxer_Test_Struct_3)

  public:
    CIEC_Struct_Muxer_Test_Struct_3();

    virtual ~CIEC_Struct_Muxer_Test_Struct_3() {
    }

  private:
    static const CStringDictionary::TStringId scm_unElementTypes[];
    static const CStringDictionary::TStringId scm_unElementNames[];
};

class CIEC_Struct_Muxer_Test_Struct_4 : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(Struct_Muxer_Test_Struct_4)

  public:
    CIEC_Struct_Muxer_Test_Struct_4();

    virtual ~CIEC_Struct_Muxer_Test_Struct_4() {
    }

  private:
    static const CStringDictionary::TStringId scm_unElementTypes[];
    static const CStringDictionary::TStringId scm_unElementNames[];
};


#endif /* TESTS_MODULES_CONVERT_STRUCT_MUX_DEMUX_DATA_H_ */
