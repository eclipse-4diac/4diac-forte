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
#include "forte_array_fixed.h"


class CIEC_Struct_Muxer_Test_Struct_1 : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(Struct_Muxer_Test_Struct_1)

  public:
    CIEC_INT Var1;
    CIEC_INT Var2;
    CIEC_STRING Var3;

    CIEC_Struct_Muxer_Test_Struct_1() = default;

    size_t getStructSize() const override {
      return 3;
    }

    const CStringDictionary::TStringId* elementNames() const override {
      return scmElementNames;
    }

    CStringDictionary::TStringId getStructTypeNameID() const override;

    CIEC_ANY *getMember(size_t paMemberIndex) override {
      switch(paMemberIndex) {
        case 0: return &Var1;
        case 1: return &Var2;
        case 2: return &Var3;
      }
      return nullptr;
    }

    const CIEC_ANY *getMember(size_t paMemberIndex) const override {
      switch(paMemberIndex) {
        case 0: return &Var1;
        case 1: return &Var2;
        case 2: return &Var3;
      }
      return nullptr;
    }
private:
    static const CStringDictionary::TStringId scmElementNames[];
};

class CIEC_Struct_Muxer_Test_Struct_2 : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(Struct_Muxer_Test_Struct_2)

  public:
    CIEC_INT Var1;
    CIEC_STRING Var2;
    CIEC_INT Var3;

    CIEC_Struct_Muxer_Test_Struct_2() = default;

    size_t getStructSize() const override {
      return 3;
    }

    const CStringDictionary::TStringId* elementNames() const override {
      return scmElementNames;
    }

    CStringDictionary::TStringId getStructTypeNameID() const override;

    CIEC_ANY *getMember(size_t paMemberIndex) override {
      switch(paMemberIndex) {
        case 0: return &Var1;
        case 1: return &Var2;
        case 2: return &Var3;
      }
      return nullptr;
    }

    const CIEC_ANY *getMember(size_t paMemberIndex) const override {
      switch(paMemberIndex) {
        case 0: return &Var1;
        case 1: return &Var2;
        case 2: return &Var3;
      }
      return nullptr;
    }
  private:
    static const CStringDictionary::TStringId scmElementNames[];
};

class CIEC_Struct_Muxer_Test_Struct_3 : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(Struct_Muxer_Test_Struct_3)

  public:
    CIEC_Struct_Muxer_Test_Struct_3() = default;

    size_t getStructSize() const override {
      return 0;
    }

    const CStringDictionary::TStringId* elementNames() const override {
      return scmElementNames;
    }

    CStringDictionary::TStringId getStructTypeNameID() const override;

    CIEC_ANY *getMember(size_t) override {
      return nullptr;
    }

    const CIEC_ANY *getMember(size_t) const override {
      return nullptr;
    }
  private:
    static const CStringDictionary::TStringId scmElementNames[];
};

class CIEC_Struct_Muxer_Test_Struct_4 : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(Struct_Muxer_Test_Struct_4)

  public:
    CIEC_INT data[256];

    CIEC_Struct_Muxer_Test_Struct_4() = default;

    size_t getStructSize() const override {
      return 256;
    }

    const CStringDictionary::TStringId* elementNames() const override {
      return scmElementNames;
    }

    CStringDictionary::TStringId getStructTypeNameID() const override;

    CIEC_ANY *getMember(size_t paMemberIndex) override {
      if(paMemberIndex < 256) {
        return &data[paMemberIndex];
      }
      return nullptr;
    }

    const CIEC_ANY *getMember(size_t paMemberIndex) const override {
      if(paMemberIndex < 256) {
        return &data[paMemberIndex];
      }
      return nullptr;
    }
  private:
    static const CStringDictionary::TStringId scmElementNames[];
};

class CIEC_Struct_Muxer_Test_Struct_5 : public CIEC_STRUCT {
DECLARE_FIRMWARE_DATATYPE(Struct_Muxer_Test_Struct_5)

public:
  CIEC_INT Var1;
  CIEC_ARRAY_FIXED<CIEC_INT, 0, 3> Var2;
  CIEC_ARRAY_FIXED<CIEC_ARRAY_FIXED<CIEC_INT, 0, 3>, 0, 1> Var3;

  CIEC_Struct_Muxer_Test_Struct_5() = default;

  size_t getStructSize() const override {
    return 3;
  }

  const CStringDictionary::TStringId* elementNames() const override {
    return scmElementNames;
  }

  CStringDictionary::TStringId getStructTypeNameID() const override;

  CIEC_ANY *getMember(size_t paMemberIndex) override {
    switch(paMemberIndex) {
      case 0: return &Var1;
      case 1: return &Var2;
      case 2: return &Var3;
    }
    return nullptr;
  }

  const CIEC_ANY *getMember(size_t paMemberIndex) const override {
    switch(paMemberIndex) {
      case 0: return &Var1;
      case 1: return &Var2;
      case 2: return &Var3;
    }
    return nullptr;
  }
private:
  static const CStringDictionary::TStringId scmElementNames[];
};


#endif /* TESTS_MODULES_CONVERT_STRUCT_MUX_DEMUX_DATA_H_ */
