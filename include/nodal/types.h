/** -*- C++ -*-
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Fabio Massaioli
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include "node_data.h"
#include "type.h"

#include <type_traits>

#include "detail/generic_type.h"

namespace nodal {

template<typename T>
class generic_type : public type
{
public:
  repr_t repr() const override final
  {
    return detail::generic_type_repr<T>();
  }

  std::size_t size() const override final
  {
    return detail::generic_type_traits<T>::size;
  }

  std::size_t alignment() const override final
  {
    return detail::generic_type_traits<T>::alignment;
  }

  bool is_vector() const override final
  {
    return detail::generic_type_traits<T>::is_vector;
  }

  std::size_t vector_size() const override final
  {
    return detail::generic_type_traits<T>::vector_size;
  }

  bool is_list() const override final
  {
    return detail::generic_type_traits<T>::is_list;
  }

  void construct(node_data* data,
                 std::size_t index,
                 bool param = false) const override final
  {
    if (param)
      new (&data->param<T>(index)) T;
    else
      new (&data->input<T>(index)) T;
  }

  void destroy(node_data* data,
               std::size_t index,
               bool param = false) const override final
  {
    if (param)
      data->param<T>(index).~T();
    else
      data->input<T>(index).~T();
  }

  bool as_bool(node_data const* data,
               std::size_t index,
               bool param = false) const override final
  {
    return get<bool>(data, index, param);
  }

  std::uint8_t as_byte(node_data const* data,
                       std::size_t index,
                       bool param = false) const override final
  {
    return get<std::uint8_t>(data, index, param);
  }

  std::intmax_t as_int(node_data const* data,
                       std::size_t index,
                       bool param = false) const override final
  {
    return get<std::intmax_t>(data, index, param);
  }

  std::uintmax_t as_uint(node_data const* data,
                         std::size_t index,
                         bool param = false) const override final
  {
    return get<std::uintmax_t>(data, index, param);
  }

  double as_real(node_data const* data,
                 std::size_t index,
                 bool param = false) const override final
  {
    return get<double>(data, index, param);
  }

  std::string as_string(node_data const* data,
                        std::size_t index,
                        bool param = false) const override final
  {
    return get<std::string>(data, index, param);
  }

  void from_bool(bool value, node_data* data,
                 std::size_t index, bool param = false) const override final
  {
    set(value, data, index, param);
  }

  void from_byte(std::uint8_t value, node_data* data,
                 std::size_t index, bool param = false) const override final
  {
    set(value, data, index, param);
  }

  void from_int(std::intmax_t value, node_data* data,
                std::size_t index, bool param = false) const override final
  {
    set(value, data, index, param);
  }

  void from_uint(std::uintmax_t value, node_data* data,
                 std::size_t index, bool param = false) const override final
  {
    set(value, data, index, param);
  }

  void from_real(double value, node_data* data,
                 std::size_t index, bool param = false) const override final
  {
    set(value, data, index, param);
  }

  void from_string(std::string const& value, node_data* data,
                   std::size_t index, bool param = false) const override final
  {
    set(value, data, index, param);
  }

protected:
  void* as_pointer(node_data const* data,
                   std::size_t index,
                   bool param = false) const override final
  {
    if (param)
      return detail::generic_type_as_pointer(data->param<T>(index));

    return detail::generic_type_as_pointer(data->input<T>(index));
  }

  void from_pointer(void* pointer,
                    node_data* data,
                    std::size_t index,
                    bool param = false) const override final
  {
    if (param)
      detail::generic_type_from_pointer(data->param<T>(index), pointer);
    else
      detail::generic_type_from_pointer(data->input<T>(index), pointer);
  }

private:
  template<typename Repr>
  Repr get(node_data const* data,
           std::size_t index,
           bool param) const
  {
    if (param)
      return detail::generic_type_cast<Repr>(data->param<T>(index));

    return detail::generic_type_cast<Repr>(data->input<T>(index));
  }

  template<typename Repr>
  void set(Repr const& value,
           node_data* data,
           std::size_t index,
           bool param) const
  {
    if (param)
      detail::generic_type_assign(data->param<T>(index), value);
    else
      detail::generic_type_assign(data->input<T>(index), value);
  }
};

#ifndef __NODAL_BUILDING_TYPES_CPP__

extern template class generic_type<bool>;

extern template class generic_type<std::int8_t>;
extern template class generic_type<std::uint8_t>;
extern template class generic_type<std::int16_t>;
extern template class generic_type<std::uint16_t>;
extern template class generic_type<std::int32_t>;
extern template class generic_type<std::uint32_t>;
extern template class generic_type<std::int64_t>;
extern template class generic_type<std::uint64_t>;

extern template class generic_type<float>;
extern template class generic_type<double>;

extern template class generic_type<std::string>;

extern template class generic_type<void*>;

#endif

namespace types
{

  class none_type : public type
  {
  public:
    repr_t repr() const override final
    {
      return none;
    }

    std::size_t size() const override final
    {
      return 0;
    }

    std::size_t alignment() const override final
    {
      return 0;
    }
  };

  using bool_type = generic_type<bool>;

  using byte_type = generic_type<std::uint8_t>;
  using int_type = generic_type<int>;
  using uint_type = generic_type<unsigned int>;

  using int16_type = generic_type<std::int16_t>;
  using uint16_type = generic_type<std::uint16_t>;
  using int32_type = generic_type<std::int32_t>;
  using uint32_type = generic_type<std::uint32_t>;
  using int64_type = generic_type<std::int64_t>;
  using uint64_type = generic_type<std::uint64_t>;

  using float_type = generic_type<float>;
  using double_type = generic_type<double>;

  using string_type = generic_type<std::string>;

  using pointer_type = generic_type<void*>;

  // Singletons

  extern none_type const none;

  extern bool_type const boolean;

  extern byte_type const byte;
  extern int_type const integer;
  extern uint_type const unsigned_int;

  extern int16_type const int16;
  extern uint16_type const uint16;
  extern int32_type const int32;
  extern uint32_type const uint32;
  extern int64_type const int64;
  extern uint64_type const uint64;

  extern float_type const real;
  extern float_type const& float_single;
  extern double_type const float_double;

  extern string_type const string;

  extern pointer_type const pointer;

}

} /* namespace nodal */
