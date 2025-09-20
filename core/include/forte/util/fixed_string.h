/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
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

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <string_view>
#include <type_traits>
#include <stdexcept>

namespace forte::util {
  /*!\brief A fixed string for use as a non-type template paramater.
   *
   * This is intended to be a drop-in replacement for the upcoming
   * std::fixed_string outlined in the C++ standardization paper
   * https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2025/p3094r6.html
   */
  template<std::size_t N>
  class fixed_string {
    public:
      using value_type = char;
      using pointer = value_type *;
      using const_pointer = const value_type *;
      using reference = value_type &;
      using const_reference = const value_type &;
      using const_iterator = const_pointer;
      using iterator = const_iterator;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;
      using reverse_iterator = const_reverse_iterator;
      using size_type = size_t;
      using difference_type = std::ptrdiff_t;

      template<std::same_as<char>... Chars>
        requires(sizeof...(Chars) == N) && (... && !std::is_pointer_v<Chars>)
      constexpr explicit fixed_string(Chars... chars) noexcept : data_{chars...} {
      }

      consteval explicit(false) fixed_string(const char (&txt)[N + 1]) noexcept {
        std::copy_n(txt, N, data_);
      }

      template<std::input_iterator It, std::sentinel_for<It> S>
        requires std::same_as<std::iter_value_t<It>, char>
      constexpr fixed_string(It begin, S end) {
        std::copy_n(begin, std::distance(begin, end), data_);
      }

      constexpr fixed_string(const fixed_string &) noexcept = default;
      constexpr fixed_string &operator=(const fixed_string &) noexcept = default;

      constexpr const_iterator begin() const noexcept {
        return data_;
      }

      constexpr const_iterator end() const noexcept {
        return data_ + size;
      }

      constexpr const_iterator cbegin() const noexcept {
        return data_;
      }

      constexpr const_iterator cend() const noexcept {
        return data_ + size;
      }

      constexpr const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator{end()};
      }

      constexpr const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator{begin()};
      }

      constexpr const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator{cend()};
      }

      constexpr const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator{cbegin()};
      }

      static constexpr std::integral_constant<size_type, N> size{};
      static constexpr std::integral_constant<size_type, N> length{};
      static constexpr std::integral_constant<size_type, N> max_size{};
      static constexpr std::bool_constant<N == 0> empty{};

      constexpr const_reference operator[](size_type pos) const {
        return data_[pos];
      }

      constexpr const_reference at(size_type pos) const {
        if (pos >= size) {
#if __cpp_exceptions
          throw std::out_of_range("fixed_string::at");
#else
          std::abort();
#endif
        }
        return data_[pos];
      }

      constexpr const_reference front() const {
        return data_[0];
      }

      constexpr const_reference back() const {
        return data_[size - 1];
      }

      constexpr void swap(fixed_string &s) noexcept {
        std::swap_ranges(begin(), end(), s.begin());
      }

      constexpr const_pointer c_str() const noexcept {
        return data_;
      }

      constexpr const_pointer data() const noexcept {
        return data_;
      }

      constexpr std::basic_string_view<char> view() const noexcept {
        return std::basic_string_view<char>{data_, size};
      }

      constexpr operator std::basic_string_view<char>() const noexcept {
        return view();
      }

      template<size_t N2>
      constexpr friend fixed_string<N + N2> operator+(const fixed_string &lhs, const fixed_string<N2> &rhs) noexcept {
        fixed_string<N + N2> result;
        std::copy_n(lhs.data_, N, result.data_);
        std::copy_n(rhs.data_, N2, result.data_ + N);
        return result;
      }

      constexpr friend fixed_string<N + 1> operator+(const fixed_string &lhs, char rhs) noexcept {
        fixed_string<N + 1> result;
        std::copy_n(lhs.data_, N, result.data_);
        result.data_[N] = rhs;
        return result;
      }

      constexpr friend fixed_string<1 + N> operator+(const char lhs, const fixed_string &rhs) noexcept {
        fixed_string<1 + N> result;
        result.data_[0] = lhs;
        std::copy_n(rhs.data_, N, result.data_ + 1);
        return result;
      }

      template<size_t N2>
      consteval friend fixed_string<N + N2 - 1> operator+(const fixed_string &lhs, const char (&rhs)[N2]) noexcept {
        fixed_string<N + N2 - 1> result;
        std::copy_n(lhs.data_, N, result.data_);
        std::copy_n(rhs, N2, result.data_ + N);
        return result;
      }

      template<size_t N1>
      consteval friend fixed_string<N1 + N - 1> operator+(const char (&lhs)[N1], const fixed_string &rhs) noexcept {
        fixed_string<N1 + N - 1> result;
        std::copy_n(lhs, N1, result.data_);
        std::copy_n(rhs.data_, N, result.data_ + N1);
        return result;
      }

      template<size_t N2>
      friend constexpr bool operator==(const fixed_string &lhs, const fixed_string<N2> &rhs) {
        return lhs.view() == rhs.view();
      }

      template<size_t N2>
      friend consteval bool operator==(const fixed_string &lhs, const char (&rhs)[N2]) {
        return lhs.view() == basic_string_view<char>(rhs, rhs + N2 - 1);
      }

      template<size_t N2>
      friend constexpr auto operator<=>(const fixed_string &lhs, const fixed_string<N2> &rhs) {
        return lhs.view() <=> rhs.view();
      }

      template<size_t N2>
      friend consteval auto operator<=>(const fixed_string &lhs, const char (&rhs)[N2]) {
        return lhs.view() <=> basic_string_view<char>(rhs, rhs + N2 - 1);
      }

      char data_[N + 1] = {}; // must currently be public to qualify as a structural type
  };

  template<std::same_as<char>... Rest>
  fixed_string(char, Rest...) -> fixed_string<1 + sizeof...(Rest)>;

  template<std::size_t N>
  fixed_string(const char (&str)[N]) -> fixed_string<N - 1>;

  template<size_t N>
  constexpr void swap(fixed_string<N> &x, fixed_string<N> &y) noexcept {
    x.swap(y);
  }
} // namespace forte::util

template<size_t N>
struct std::hash<forte::util::fixed_string<N>> : hash<std::string_view> {};
