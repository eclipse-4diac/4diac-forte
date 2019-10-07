/*******************************************************************************
 * Copyright (c) 2018 Johannes Kepler University
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "ForteLibrary.h"

#include "Arp/System/Core/TypeName.hxx"
#include "ForteComponent.h"

namespace ForteLibrary{

ForteLibrary::ForteLibrary(AppDomain& paAppDomain) : LibraryBase(paAppDomain) {
    componentFactory.AddFactoryMethod("ForteComponent", &ForteComponent::Create);
}

void ForteLibrary::Main(AppDomain& paAppDomain){
    TSingletonBase::CreateInstance(paAppDomain);
}

ILibrary* ForteLibrary::GetInstance(){
    return &TSingletonBase::GetInstance();
}

extern "C" ARP_CXX_SYMBOL_EXPORT void DynamicLibrary_Main(AppDomain& appDomain){
    ForteLibrary::Main(appDomain);
}

extern "C" ARP_CXX_SYMBOL_EXPORT ILibrary* DynamicLibrary_GetInstance(){
    return ForteLibrary::GetInstance();
}

} // end of namespace ForteLibrary
