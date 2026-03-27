/*******************************************************************************
 * Copyright (c) 2026 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Erich Jobst - initial implementation
 *******************************************************************************/

#include <boost/test/unit_test.hpp>

#include "forte_boost_output_support.h"

#include "forte/datatypes/forte_any_derived_variant.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"

using namespace forte::literals;

namespace forte::test {
  class CIEC_AnyDerivedTestStruct : public CIEC_STRUCT {
      DECLARE_FIRMWARE_DATATYPE(AnyDerivedTestStruct)
    public:
      CIEC_STRING Var1;
      CIEC_BOOL Var2;
      CIEC_INT Var3;

      CIEC_AnyDerivedTestStruct() = default;

      CIEC_AnyDerivedTestStruct(CIEC_STRING paVar1, CIEC_BOOL paVar2, CIEC_INT paVar3) :
          Var1(paVar1),
          Var2(paVar2),
          Var3(paVar3) {};

      size_t getStructSize() const override {
        return 3;
      }

      const StringId *elementNames() const override {
        return scmElementNames;
      }

      StringId getStructTypeNameID() const override {
        return "AnyDerivedTestStruct"_STRID;
      }

      CIEC_ANY *getMember(size_t paMemberIndex) override {
        switch (paMemberIndex) {
          case 0: return &Var1;
          case 1: return &Var2;
          case 2: return &Var3;
        }
        return nullptr;
      }

      const CIEC_ANY *getMember(size_t paMemberIndex) const override {
        switch (paMemberIndex) {
          case 0: return &Var1;
          case 1: return &Var2;
          case 2: return &Var3;
        }
        return nullptr;
      }

    private:
      static const StringId scmElementNames[];
  };

  const StringId CIEC_AnyDerivedTestStruct::scmElementNames[] = {"Var1"_STRID, "Var2"_STRID, "Var3"_STRID};

  DEFINE_FIRMWARE_DATATYPE(AnyDerivedTestStruct, "AnyDerivedTestStruct"_STRID)

  BOOST_AUTO_TEST_SUITE(CIEC_ANY_DERIVED_VARIANT_function_test)

  BOOST_AUTO_TEST_CASE(Type_test) {
    const CIEC_ANY_DERIVED_VARIANT test;
    BOOST_CHECK(test.getDataTypeID() == CIEC_ANY::e_ANY);
  }

  BOOST_AUTO_TEST_CASE(Initialization_test) {
    // assign struct value
    CIEC_ANY_DERIVED_VARIANT test1 = CIEC_AnyDerivedTestStruct("test"_STRING, true_BOOL, 17_INT);
    auto test1Value = static_cast<CIEC_AnyDerivedTestStruct &>(test1.unwrap());
    BOOST_TEST("test" == static_cast<CIEC_STRING::TValueType>(test1Value.Var1));
    BOOST_TEST(true == static_cast<CIEC_BOOL::TValueType>(test1Value.Var2));
    BOOST_TEST(17 == static_cast<CIEC_INT::TValueType>(test1Value.Var3));

    // assign generic struct value
    CIEC_ANY_DERIVED_VARIANT test3 = CIEC_AnyDerivedTestStruct("other"_STRING, false_BOOL, 4_INT);
    CIEC_ANY &test4 = test3;
    CIEC_ANY_DERIVED_VARIANT test2 = test4;
    auto test2Value = static_cast<CIEC_AnyDerivedTestStruct &>(test2.unwrap());
    BOOST_TEST("other" == static_cast<CIEC_STRING::TValueType>(test2Value.Var1));
    BOOST_TEST(false == static_cast<CIEC_BOOL::TValueType>(test2Value.Var2));
    BOOST_TEST(4 == static_cast<CIEC_INT::TValueType>(test2Value.Var3));

    // assign structs to each other
    CIEC_ANY_DERIVED_VARIANT test5 = test1;
    auto test5Value = static_cast<CIEC_AnyDerivedTestStruct &>(test5.unwrap());
    BOOST_TEST("test" == static_cast<CIEC_STRING::TValueType>(test5Value.Var1));
    BOOST_TEST(true == static_cast<CIEC_BOOL::TValueType>(test5Value.Var2));
    BOOST_TEST(17 == static_cast<CIEC_INT::TValueType>(test5Value.Var3));

    // assign array value
    CIEC_ANY_DERIVED_VARIANT test6 = CIEC_ARRAY_FIXED<CIEC_DINT, 0, 2>{17_DINT, 4_DINT, 21_DINT};
    auto test6Value = static_cast<CIEC_ARRAY_FIXED<CIEC_DINT, 0, 2> &>(test6.unwrap());
    BOOST_TEST(17 == static_cast<CIEC_DINT::TValueType>(test6Value[0]));
    BOOST_TEST(4 == static_cast<CIEC_DINT::TValueType>(test6Value[1]));
    BOOST_TEST(21 == static_cast<CIEC_DINT::TValueType>(test6Value[2]));

    // assign generic array value
    CIEC_ANY_DERIVED_VARIANT test8 = CIEC_ARRAY_FIXED<CIEC_DINT, 0, 2>{1_DINT, 2_DINT, 4_DINT};
    CIEC_ANY &test9 = test8;
    CIEC_ANY_DERIVED_VARIANT test7 = test9;
    auto test7Value = static_cast<CIEC_ARRAY_FIXED<CIEC_DINT, 0, 2> &>(test7.unwrap());
    BOOST_TEST(1 == static_cast<CIEC_DINT::TValueType>(test7Value[0]));
    BOOST_TEST(2 == static_cast<CIEC_DINT::TValueType>(test7Value[1]));
    BOOST_TEST(4 == static_cast<CIEC_DINT::TValueType>(test7Value[2]));

    // assign arrays to each other
    CIEC_ANY_DERIVED_VARIANT test10 = test6;
    auto test10Value = static_cast<CIEC_ARRAY_FIXED<CIEC_DINT, 0, 2> &>(test10.unwrap());
    BOOST_TEST(17 == static_cast<CIEC_DINT::TValueType>(test10Value[0]));
    BOOST_TEST(4 == static_cast<CIEC_DINT::TValueType>(test10Value[1]));
    BOOST_TEST(21 == static_cast<CIEC_DINT::TValueType>(test10Value[2]));
  }

  BOOST_AUTO_TEST_CASE(Assignment_test) {
    CIEC_ANY_DERIVED_VARIANT test1;
    CIEC_ANY_DERIVED_VARIANT test2;

    // initial value must be ANY_STRUCT
    BOOST_TEST(CIEC_ANY::EDataTypeID::e_STRUCT == test1.unwrap().getDataTypeID());
    BOOST_TEST(CIEC_ANY::EDataTypeID::e_STRUCT == test2.unwrap().getDataTypeID());
    BOOST_TEST("ANY_STRUCT" == static_cast<CIEC_STRUCT &>(test1.unwrap()).getStructTypeNameID().get());
    BOOST_TEST("ANY_STRUCT" == static_cast<CIEC_STRUCT &>(test2.unwrap()).getStructTypeNameID().get());

    // assign struct value
    test1 = CIEC_AnyDerivedTestStruct("test"_STRING, true_BOOL, 17_INT);
    auto test1Value = static_cast<CIEC_AnyDerivedTestStruct &>(test1.unwrap());
    BOOST_TEST("test" == static_cast<CIEC_STRING::TValueType>(test1Value.Var1));
    BOOST_TEST(true == static_cast<CIEC_BOOL::TValueType>(test1Value.Var2));
    BOOST_TEST(17 == static_cast<CIEC_INT::TValueType>(test1Value.Var3));

    // assign generic struct value
    CIEC_AnyDerivedTestStruct test3 = CIEC_AnyDerivedTestStruct("other"_STRING, false_BOOL, 4_INT);
    CIEC_ANY &test4 = test3;
    test2 = test4;
    auto test2Value = static_cast<CIEC_AnyDerivedTestStruct &>(test2.unwrap());
    BOOST_TEST("other" == static_cast<CIEC_STRING::TValueType>(test2Value.Var1));
    BOOST_TEST(false == static_cast<CIEC_BOOL::TValueType>(test2Value.Var2));
    BOOST_TEST(4 == static_cast<CIEC_INT::TValueType>(test2Value.Var3));

    // assign each other
    test2.setValue(test1);
    test2Value = static_cast<CIEC_AnyDerivedTestStruct &>(test2.unwrap());
    BOOST_TEST("test" == static_cast<CIEC_STRING::TValueType>(test2Value.Var1));
    BOOST_TEST(true == static_cast<CIEC_BOOL::TValueType>(test2Value.Var2));
    BOOST_TEST(17 == static_cast<CIEC_INT::TValueType>(test2Value.Var3));

    // assign dynamic value
    test2.setValue(CIEC_AnyDerivedTestStruct("other"_STRING, false_BOOL, 4_INT));
    test2Value = static_cast<CIEC_AnyDerivedTestStruct &>(test2.unwrap());
    BOOST_TEST("other" == static_cast<CIEC_STRING::TValueType>(test2Value.Var1));
    BOOST_TEST(false == static_cast<CIEC_BOOL::TValueType>(test2Value.Var2));
    BOOST_TEST(4 == static_cast<CIEC_INT::TValueType>(test2Value.Var3));

    // assign array value
    test1 = CIEC_ARRAY_FIXED<CIEC_DINT, 0, 2>{17_DINT, 4_DINT, 21_DINT};
    auto test1ArrayValue = static_cast<CIEC_ARRAY_FIXED<CIEC_DINT, 0, 2> &>(test1.unwrap());
    BOOST_TEST(17 == static_cast<CIEC_DINT::TValueType>(test1ArrayValue[0]));
    BOOST_TEST(4 == static_cast<CIEC_DINT::TValueType>(test1ArrayValue[1]));
    BOOST_TEST(21 == static_cast<CIEC_DINT::TValueType>(test1ArrayValue[2]));

    // assign generic array value
    CIEC_ARRAY_FIXED<CIEC_DINT, 0, 2> test5 = CIEC_ARRAY_FIXED<CIEC_DINT, 0, 2>{1_DINT, 2_DINT, 4_DINT};
    CIEC_ANY &test6 = test5;
    test2 = test6;
    auto test2ArrayValue = static_cast<CIEC_ARRAY_FIXED<CIEC_DINT, 0, 2> &>(test2.unwrap());
    BOOST_TEST(1 == static_cast<CIEC_DINT::TValueType>(test2ArrayValue[0]));
    BOOST_TEST(2 == static_cast<CIEC_DINT::TValueType>(test2ArrayValue[1]));
    BOOST_TEST(4 == static_cast<CIEC_DINT::TValueType>(test2ArrayValue[2]));

    // assign each other
    test2.setValue(test1);
    test2ArrayValue = static_cast<CIEC_ARRAY_FIXED<CIEC_DINT, 0, 2> &>(test2.unwrap());
    BOOST_TEST(17 == static_cast<CIEC_DINT::TValueType>(test2ArrayValue[0]));
    BOOST_TEST(4 == static_cast<CIEC_DINT::TValueType>(test2ArrayValue[1]));
    BOOST_TEST(21 == static_cast<CIEC_DINT::TValueType>(test2ArrayValue[2]));

    // assign dynamic value
    test2.setValue(CIEC_ARRAY_FIXED<CIEC_DINT, 0, 2>{1_DINT, 2_DINT, 4_DINT});
    test2ArrayValue = static_cast<CIEC_ARRAY_FIXED<CIEC_DINT, 0, 2> &>(test2.unwrap());
    BOOST_TEST(1 == static_cast<CIEC_DINT::TValueType>(test2ArrayValue[0]));
    BOOST_TEST(2 == static_cast<CIEC_DINT::TValueType>(test2ArrayValue[1]));
    BOOST_TEST(4 == static_cast<CIEC_DINT::TValueType>(test2ArrayValue[2]));
  }

  void
  checkStringConversion(CIEC_ANY_DERIVED_VARIANT &paValue, const char *paString, CIEC_ANY::EDataTypeID paDataTypeId) {
    std::string buf;
    BOOST_CHECK_EQUAL(paValue.fromString(paString), strlen(paString));
    BOOST_CHECK_EQUAL(paValue.getDataTypeID(), CIEC_ANY::e_ANY);
    BOOST_CHECK_EQUAL(paValue.unwrap().getDataTypeID(), paDataTypeId);
    paValue.toString(buf);
    BOOST_CHECK_EQUAL(paString, buf);
  }

  BOOST_AUTO_TEST_CASE(String_Conversion_test) {
    CIEC_ANY_DERIVED_VARIANT test;
    checkStringConversion(test, "AnyDerivedTestStruct#(Var1:='test',Var2:=TRUE,Var3:=17)", CIEC_ANY::e_STRUCT);
  }

  BOOST_AUTO_TEST_CASE(Equality_test) {
    CIEC_ANY_DERIVED_VARIANT test1;
    CIEC_ANY_DERIVED_VARIANT test2;

    BOOST_TEST(test1.equals(test2));
    test1 = CIEC_AnyDerivedTestStruct("test"_STRING, true_BOOL, 17_INT);
    BOOST_TEST(!test1.equals(test2));
    test2.setValue(test1);
    BOOST_TEST(test1.equals(test2));
    test2.setValue(CIEC_AnyDerivedTestStruct("other"_STRING, false_BOOL, 4_INT));
    BOOST_TEST(!test1.equals(test2));

    test1 = CIEC_ARRAY_FIXED<CIEC_DINT, 0, 2>{17_DINT, 4_DINT, 21_DINT};
    BOOST_TEST(!test1.equals(test2));
    test2.setValue(test1);
    BOOST_TEST(test1.equals(test2));
    test2.setValue(CIEC_ARRAY_FIXED<CIEC_DINT, 0, 2>{1_DINT, 2_DINT, 4_DINT});
    BOOST_TEST(!test1.equals(test2));
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
