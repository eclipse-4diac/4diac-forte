/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/
#pragma once

#include <memory>

#include "forte_any.h"

template<typename T>
class CIEC_ANY_UNIQUE_PTR : public std::unique_ptr<T> {
public:
    using std::unique_ptr<T>::unique_ptr;
    using std::unique_ptr<T>::operator=;

    CIEC_ANY_UNIQUE_PTR(const CIEC_ANY_UNIQUE_PTR &paOther) :
            std::unique_ptr<T>(static_cast<T *>(paOther->clone(nullptr))) {}

    CIEC_ANY_UNIQUE_PTR &operator=(const CIEC_ANY_UNIQUE_PTR &paOther) {
      std::unique_ptr<T>::operator=(std::unique_ptr<T>(static_cast<T *>(paOther->clone(nullptr))));
      return *this;
    }

    ~CIEC_ANY_UNIQUE_PTR() = default;
};

static_assert(std::is_copy_constructible_v<CIEC_ANY_UNIQUE_PTR<CIEC_ANY>>);
static_assert(std::is_move_constructible_v<CIEC_ANY_UNIQUE_PTR<CIEC_ANY>>);
static_assert(std::is_copy_assignable_v<CIEC_ANY_UNIQUE_PTR<CIEC_ANY>>);
static_assert(std::is_move_assignable_v<CIEC_ANY_UNIQUE_PTR<CIEC_ANY>>);

