/*******************************************************************************
 * Copyright (c) 2018 TU Wien/ACIN
 *               2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *    Martin Jobst
 *      - account for data type size in FB initialization
 *******************************************************************************/

#include "simplefb.h"

CSimpleFB::CSimpleFB(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
                     CStringDictionary::TStringId paInstanceNameId, const SInternalVarsInformation *paVarInternals,
                     TForteByte *paFBConnData, TForteByte *paBasicFBVarsData) : CBasicFB(paSrcRes, paInterfaceSpec,
                                                                                         paInstanceNameId,
                                                                                         paVarInternals, paFBConnData,
                                                                                         paBasicFBVarsData) {}
