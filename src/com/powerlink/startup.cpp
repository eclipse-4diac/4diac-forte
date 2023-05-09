/*******************************************************************************
 * Copyright (c) 2023 Primetals Technolgies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - migrated from posix main
 *******************************************************************************/

#include <EplWrapper.h>

void powerlinkStartupHook(int argc, char *arg[]){
  CEplStackWrapper::eplMainInit();
}
