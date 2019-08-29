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
#pragma once
#include <Arp/System/Core/Arp.h>
#include <Arp/System/Core/AppDomain.hpp>
#include <Arp/System/Core/Singleton.hxx>
#include <Arp/System/Core/Library.h>
#include <Arp/System/Acf/LibraryBase.hpp>

using namespace Arp;
using namespace Arp::System::Acf;

namespace ForteLibrary{

class ForteLibrary : public LibraryBase, public Singleton<ForteLibrary> {
  public:
    typedef Singleton<ForteLibrary> TSingletonBase;

    explicit ForteLibrary(AppDomain& appDomain);
    virtual ~ForteLibrary(void) = default;

    static void Main(AppDomain& appDomain);
    static ILibrary* GetInstance(void);

  private:
    ForteLibrary(const ForteLibrary& arg) = delete;
    ForteLibrary& operator= (const ForteLibrary& arg) = delete;
};

extern "C" ARP_CXX_SYMBOL_EXPORT void DynamicLibrary_Main(AppDomain& appDomain);
extern "C" ARP_CXX_SYMBOL_EXPORT ILibrary* DynamicLibrary_GetInstance(void);

} // end of namespace ForteLibrary
