/*******************************************************************************
 * Copyright (c) 2018 Johannes Kepler University
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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
