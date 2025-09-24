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
#include "struct_mux_demux_data.h"

using namespace forte::literals;

namespace forte::eclipse4diac::convert::test {
  DEFINE_FIRMWARE_DATATYPE(Struct_Muxer_Test_Struct_1, "Struct_Muxer_Test_Struct_1"_STRID)
  const StringId CIEC_Struct_Muxer_Test_Struct_1::scmElementNames[] = {"Var1"_STRID, "Var2"_STRID, "Var3"_STRID};

  StringId CIEC_Struct_Muxer_Test_Struct_1::getStructTypeNameID() const {
    return "Struct_Muxer_Test_Struct_1"_STRID;
  };

  DEFINE_FIRMWARE_DATATYPE(Struct_Muxer_Test_Struct_2, "Struct_Muxer_Test_Struct_2"_STRID)
  const StringId CIEC_Struct_Muxer_Test_Struct_2::scmElementNames[] = {"Var1"_STRID, "Var2"_STRID, "Var3"_STRID};

  StringId CIEC_Struct_Muxer_Test_Struct_2::getStructTypeNameID() const {
    return "Struct_Muxer_Test_Struct_2"_STRID;
  };

  DEFINE_FIRMWARE_DATATYPE(Struct_Muxer_Test_Struct_3, "Struct_Muxer_Test_Struct_3"_STRID)
  const StringId CIEC_Struct_Muxer_Test_Struct_3::scmElementNames[] = {{}};

  StringId CIEC_Struct_Muxer_Test_Struct_3::getStructTypeNameID() const {
    return "Struct_Muxer_Test_Struct_3"_STRID;
  };

  DEFINE_FIRMWARE_DATATYPE(Struct_Muxer_Test_Struct_4, "Struct_Muxer_Test_Struct_4"_STRID)
  const StringId CIEC_Struct_Muxer_Test_Struct_4::scmElementNames[] = {
      "Var0"_STRID,   "Var1"_STRID,   "Var2"_STRID,   "Var3"_STRID,   "Var4"_STRID,   "Var5"_STRID,   "Var6"_STRID,
      "Var7"_STRID,   "Var8"_STRID,   "Var9"_STRID,   "Var10"_STRID,  "Var11"_STRID,  "Var12"_STRID,  "Var13"_STRID,
      "Var14"_STRID,  "Var15"_STRID,  "Var16"_STRID,  "Var17"_STRID,  "Var18"_STRID,  "Var19"_STRID,  "Var20"_STRID,
      "Var21"_STRID,  "Var22"_STRID,  "Var23"_STRID,  "Var24"_STRID,  "Var25"_STRID,  "Var26"_STRID,  "Var27"_STRID,
      "Var28"_STRID,  "Var29"_STRID,  "Var30"_STRID,  "Var31"_STRID,  "Var32"_STRID,  "Var33"_STRID,  "Var34"_STRID,
      "Var35"_STRID,  "Var36"_STRID,  "Var37"_STRID,  "Var38"_STRID,  "Var39"_STRID,  "Var40"_STRID,  "Var41"_STRID,
      "Var42"_STRID,  "Var43"_STRID,  "Var44"_STRID,  "Var45"_STRID,  "Var46"_STRID,  "Var47"_STRID,  "Var48"_STRID,
      "Var49"_STRID,  "Var50"_STRID,  "Var51"_STRID,  "Var52"_STRID,  "Var53"_STRID,  "Var54"_STRID,  "Var55"_STRID,
      "Var56"_STRID,  "Var57"_STRID,  "Var58"_STRID,  "Var59"_STRID,  "Var60"_STRID,  "Var61"_STRID,  "Var62"_STRID,
      "Var63"_STRID,  "Var64"_STRID,  "Var65"_STRID,  "Var66"_STRID,  "Var67"_STRID,  "Var68"_STRID,  "Var69"_STRID,
      "Var70"_STRID,  "Var71"_STRID,  "Var72"_STRID,  "Var73"_STRID,  "Var74"_STRID,  "Var75"_STRID,  "Var76"_STRID,
      "Var77"_STRID,  "Var78"_STRID,  "Var79"_STRID,  "Var80"_STRID,  "Var81"_STRID,  "Var82"_STRID,  "Var83"_STRID,
      "Var84"_STRID,  "Var85"_STRID,  "Var86"_STRID,  "Var87"_STRID,  "Var88"_STRID,  "Var89"_STRID,  "Var90"_STRID,
      "Var91"_STRID,  "Var92"_STRID,  "Var93"_STRID,  "Var94"_STRID,  "Var95"_STRID,  "Var96"_STRID,  "Var97"_STRID,
      "Var98"_STRID,  "Var99"_STRID,  "Var100"_STRID, "Var101"_STRID, "Var102"_STRID, "Var103"_STRID, "Var104"_STRID,
      "Var105"_STRID, "Var106"_STRID, "Var107"_STRID, "Var108"_STRID, "Var109"_STRID, "Var110"_STRID, "Var111"_STRID,
      "Var112"_STRID, "Var113"_STRID, "Var114"_STRID, "Var115"_STRID, "Var116"_STRID, "Var117"_STRID, "Var118"_STRID,
      "Var119"_STRID, "Var120"_STRID, "Var121"_STRID, "Var122"_STRID, "Var123"_STRID, "Var124"_STRID, "Var125"_STRID,
      "Var126"_STRID, "Var127"_STRID, "Var128"_STRID, "Var129"_STRID, "Var130"_STRID, "Var131"_STRID, "Var132"_STRID,
      "Var133"_STRID, "Var134"_STRID, "Var135"_STRID, "Var136"_STRID, "Var137"_STRID, "Var138"_STRID, "Var139"_STRID,
      "Var140"_STRID, "Var141"_STRID, "Var142"_STRID, "Var143"_STRID, "Var144"_STRID, "Var145"_STRID, "Var146"_STRID,
      "Var147"_STRID, "Var148"_STRID, "Var149"_STRID, "Var150"_STRID, "Var151"_STRID, "Var152"_STRID, "Var153"_STRID,
      "Var154"_STRID, "Var155"_STRID, "Var156"_STRID, "Var157"_STRID, "Var158"_STRID, "Var159"_STRID, "Var160"_STRID,
      "Var161"_STRID, "Var162"_STRID, "Var163"_STRID, "Var164"_STRID, "Var165"_STRID, "Var166"_STRID, "Var167"_STRID,
      "Var168"_STRID, "Var169"_STRID, "Var170"_STRID, "Var171"_STRID, "Var172"_STRID, "Var173"_STRID, "Var174"_STRID,
      "Var175"_STRID, "Var176"_STRID, "Var177"_STRID, "Var178"_STRID, "Var179"_STRID, "Var180"_STRID, "Var181"_STRID,
      "Var182"_STRID, "Var183"_STRID, "Var184"_STRID, "Var185"_STRID, "Var186"_STRID, "Var187"_STRID, "Var188"_STRID,
      "Var189"_STRID, "Var190"_STRID, "Var191"_STRID, "Var192"_STRID, "Var193"_STRID, "Var194"_STRID, "Var195"_STRID,
      "Var196"_STRID, "Var197"_STRID, "Var198"_STRID, "Var199"_STRID, "Var200"_STRID, "Var201"_STRID, "Var202"_STRID,
      "Var203"_STRID, "Var204"_STRID, "Var205"_STRID, "Var206"_STRID, "Var207"_STRID, "Var208"_STRID, "Var209"_STRID,
      "Var210"_STRID, "Var211"_STRID, "Var212"_STRID, "Var213"_STRID, "Var214"_STRID, "Var215"_STRID, "Var216"_STRID,
      "Var217"_STRID, "Var218"_STRID, "Var219"_STRID, "Var220"_STRID, "Var221"_STRID, "Var222"_STRID, "Var223"_STRID,
      "Var224"_STRID, "Var225"_STRID, "Var226"_STRID, "Var227"_STRID, "Var228"_STRID, "Var229"_STRID, "Var230"_STRID,
      "Var231"_STRID, "Var232"_STRID, "Var233"_STRID, "Var234"_STRID, "Var235"_STRID, "Var236"_STRID, "Var237"_STRID,
      "Var238"_STRID, "Var239"_STRID, "Var240"_STRID, "Var241"_STRID, "Var242"_STRID, "Var243"_STRID, "Var244"_STRID,
      "Var245"_STRID, "Var246"_STRID, "Var247"_STRID, "Var248"_STRID, "Var249"_STRID, "Var250"_STRID, "Var251"_STRID,
      "Var252"_STRID, "Var253"_STRID, "Var254"_STRID, "Var255"_STRID};

  StringId CIEC_Struct_Muxer_Test_Struct_4::getStructTypeNameID() const {
    return "Struct_Muxer_Test_Struct_4"_STRID;
  };

  DEFINE_FIRMWARE_DATATYPE(Struct_Muxer_Test_Struct_5, "Struct_Muxer_Test_Struct_5"_STRID)
  const StringId CIEC_Struct_Muxer_Test_Struct_5::scmElementNames[] = {"Var1"_STRID, "Var2"_STRID, "Var3"_STRID};

  StringId CIEC_Struct_Muxer_Test_Struct_5::getStructTypeNameID() const {
    return "Struct_Muxer_Test_Struct_5"_STRID;
  };
} // namespace forte::eclipse4diac::convert::test
