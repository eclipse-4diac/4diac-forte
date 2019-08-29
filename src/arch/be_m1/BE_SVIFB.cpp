/*******************************************************************************
 * Copyright (c) 2011 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "BE_SVIFB.h"
#include "BE_RMT_DEV.h"

const TForteUInt32 BE_SVIFB::scm_anBE_SVI_Types[] = { SVI_F_UNKNOWN, // e_ANY,
    SVI_F_BOOL8, // e_BOOL,
    SVI_F_SINT8, //  e_SINT,
    SVI_F_SINT16, //  e_INT,
    SVI_F_SINT32, //  e_DINT,
    SVI_F_SINT64, //  e_LINT,
    SVI_F_UINT8, //  e_USINT,
    SVI_F_UINT16, //  e_UINT,
    SVI_F_UINT32, //  e_UDINT,
    SVI_F_UINT64, //  e_ULINT,
    SVI_F_UINT8, //  e_BYTE,
    SVI_F_UINT16, //  e_WORD,
    SVI_F_UINT32, //  e_DWORD,
    SVI_F_UINT64, //  e_LWORD,
    SVI_F_SINT64, //  e_DATE,
    SVI_F_SINT64, //  e_TIME_OF_DAY,
    SVI_F_SINT64, //  e_DATE_AND_TIME,
    SVI_F_SINT64, //  e_TIME, //until here simple Datatypes
    SVI_F_REAL32, //  e_REAL,
    SVI_F_REAL64, // e_LREAL,
    SVI_F_STRINGLSTBASE, //  e_STRING,
    SVI_F_STRINGLSTBASE, //  e_WSTRING,
    SVI_F_UNKNOWN, //  e_DerivedData,
    SVI_F_UNKNOWN, //  e_DirectlyDerivedData,
    SVI_F_UNKNOWN, //  e_EnumeratedData,
    SVI_F_UNKNOWN, //  e_SubrangeData,
    SVI_F_UNKNOWN, //  e_ARRAY, //according to the compliance profile
    SVI_F_UNKNOWN //  e_STRUCT,
    //     e_External = 256, // Base for CIEC_ANY based types outside of the forte base
    //     e_Max = 65535     // Guarantees at least 16 bits - otherwise gcc will optimizes on some platforms
    };

const TForteUInt32 BE_SVIFB::scm_anBE_SVI_VarSizes[] = { 0, // e_ANY,
    1, // e_BOOL,
    1, //  e_SINT,
    2, //  e_INT,
    4, //  e_DINT,
    8, //  e_LINT,
    1, //  e_USINT,
    2, //  e_UINT,
    4, //  e_UDINT,
    8, //  e_ULINT,
    1, //  e_BYTE,
    2, //  e_WORD,
    4, //  e_DWORD,
    8, //  e_LWORD,
    8, //  e_DATE,
    8, //  e_TIME_OF_DAY,
    8, //  e_DATE_AND_TIME,
    8, //  e_TIME, //until here simple Datatypes
    4, //  e_REAL,
    8, // e_LREAL,
    4, //  e_STRING,
    4, //  e_WSTRING,
    0, //  e_DerivedData,
    0, //  e_DirectlyDerivedData,
    0, //  e_EnumeratedData,
    0, //  e_SubrangeData,
    0, //  e_ARRAY, //according to the compliance profile
    0 //  e_STRUCT,
    //     e_External = 256, // Base for CIEC_ANY based types outside of the forte base
    //     e_Max = 65535     // Guarantees at least 16 bits - otherwise gcc will optimizes on some platforms
    };

bool BE_SVIFB::registerSVIvar(const CIEC_STRING& pa_roID, CIEC_ANY& pa_roValue, const TForteUInt32 pa_nMode){
  BE_RMT_DEV& roDev(static_cast<BE_RMT_DEV&>(getResource().getDevice()));

  bool bRetVal = (OK == roDev.getModule().GetSVIHandler().AddGlobVar(((CHAR *) pa_roID.getValue()), pa_nMode | scm_anBE_SVI_Types[pa_roValue.getDataTypeID()], scm_anBE_SVI_VarSizes[pa_roValue.getDataTypeID()], (UINT32*) (pa_roValue.getDataPtr()), 0, 0));
  roDev.getModule().CfgRead();
  return bRetVal;
}
