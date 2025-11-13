/*******************************************************************************
 * Copyright (c) 2022, 2025 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Jobst
 *      - initial implementation
 *******************************************************************************/

#pragma once

#include <concepts>
#include <type_traits>

#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_any_bit.h"
#include "forte/datatypes/forte_any_bit_variant.h"
#include <forte/iec61131_functions.h>

namespace forte {
  template<typename T>
  T &ST_IGNORE_OUT_PARAM(T &&value) {
    return value;
  }

  /** @brief Extends lifetime of a temporary rvalue for function calls
   *
   * This extended the percieved lifetime of a temporary object for the compiler.
   * Its needed for Partials and negation of output parameters.
   *
   * Its the same as ST_IGNORE_OUT_PARAM, but a different name, as it has a different
   * reason in the code. Currently, aliasing of template functions does not work, so the
   * code was duplicated.
   */
  template<typename T>
  T &ST_EXTEND_LIFETIME(T &&value) {
    return value;
  }

  template<typename T>
  class COutputGuard;

  template<typename T>
    requires(std::default_initializable<T> && (!std::derived_from<T, CIEC_ANY_BIT> || forte::generic_datatype<T>) )
  class COutputParameter {
      friend COutputGuard<COutputParameter>;

    public:
      COutputParameter() : mOutput(nullptr) {
      }

      template<typename U>
        requires(std::derived_from<std::remove_reference_t<U>, CIEC_ANY> &&
                 (std::is_assignable_v<std::remove_reference_t<U>, T> ||
                  (forte::generic_datatype<T> && std::is_assignable_v<T, std::remove_reference_t<U>>) ))
      explicit(false) COutputParameter(U &&paOutput) : mOutput(&paOutput) {
      }

      T *get() noexcept {
        return &mValue;
      }

      T &operator*() noexcept {
        return mValue;
      }

      T *operator->() noexcept {
        return &mValue;
      }

    private:
      void writeBack() {
        if (mOutput) {
          mOutput->setValue(mValue.unwrap());
        }
      }

      T mValue;
      CIEC_ANY *mOutput;
  };

  template<typename T>
    requires(std::default_initializable<T> && std::derived_from<T, CIEC_ANY_BIT> && !forte::generic_datatype<T>)
  class CAnyBitOutputParameter {
      friend COutputGuard<CAnyBitOutputParameter>;

    public:
      CAnyBitOutputParameter() : mOutput(nullptr), mNegate(false) {
      }

      template<typename U>
        requires(std::derived_from<std::remove_reference_t<U>, CIEC_ANY> &&
                 std::is_assignable_v<std::remove_reference_t<U>, T>)
      explicit(false) CAnyBitOutputParameter(U &&paOutput) : mOutput(&paOutput), mNegate(false) {
      }

      template<typename U>
        requires(std::derived_from<std::remove_reference_t<U>, CIEC_ANY> &&
                 std::is_assignable_v<std::remove_reference_t<U>, T>)
      CAnyBitOutputParameter(U &&paOutput, const bool paNegate) : mOutput(&paOutput), mNegate(paNegate) {
      }

      T *get() noexcept {
        return &mValue;
      }

      T &operator*() noexcept {
        return mValue;
      }

      T *operator->() noexcept {
        return &mValue;
      }

    private:
      void writeBack() {
        if (mOutput) {
          if (mNegate) {
            mOutput->setValue(func_NOT(mValue));
          } else {
            mOutput->setValue(mValue);
          }
        }
      }

      T mValue;
      CIEC_ANY *mOutput;
      bool mNegate;
  };

  template<typename T>
  class COutputGuard {
    public:
      explicit COutputGuard(T &paParameter) : mParameter(paParameter) {
      }

      ~COutputGuard() {
        mParameter.writeBack();
      }

    private:
      T &mParameter;
  };
} // namespace forte
