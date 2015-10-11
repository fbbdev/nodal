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

#include <cstdint>
#include <string>
#include <vector>

namespace nodal
{

class node_data;

class type
{
public:
  enum repr_t
  {
    none,
    boolean,
    byte,
    integer,
    unsigned_int,
    real,
    string,
    object
  };

  virtual ~type() {}

  virtual repr_t repr() const = 0;

  virtual std::size_t size() const = 0;

  virtual std::size_t alignment() const = 0;

  virtual bool is_vector() const
  {
    return false;
  }

  virtual std::size_t vector_size() const { return 0; }

  virtual bool is_list() const
  {
    return false;
  }

  virtual void construct(node_data* data,
                         std::size_t index,
                         bool param = false) const {}

  virtual void destroy(node_data* data,
                       std::size_t index,
                       bool param = false) const {}

  virtual bool as_bool(node_data const* data,
                       std::size_t index,
                       bool param = false) const
  {
    return false;
  }

  virtual std::uint8_t as_byte(node_data const* data,
                               std::size_t index,
                               bool param = false) const
  {
    return 0;
  }

  virtual std::intmax_t as_int(node_data const* data,
                               std::size_t index,
                               bool param = false) const
  {
    return 0;
  }

  virtual std::uintmax_t as_uint(node_data const* data,
                                 std::size_t index,
                                 bool param = false) const
  {
    return 0;
  }

  virtual double as_real(node_data const* data,
                         std::size_t index,
                         bool param = false) const
  {
    return 0.0;
  }

  virtual std::string as_string(node_data const* data,
                                std::size_t index,
                                bool param = false) const
  {
    return {};
  }

  template<typename Object>
  Object* as_object(node_data const* data,
                    std::size_t index,
                    bool param = false) const
  {
    return reinterpret_cast<Object*>(as_pointer(data, index, param));
  }

  virtual void from_bool(bool value, node_data* data,
                         std::size_t index, bool param = false) const {}

  virtual void from_byte(std::uint8_t value, node_data* data,
                         std::size_t index, bool param = false) const {}

  virtual void from_int(std::intmax_t value, node_data* data,
                        std::size_t index, bool param = false) const {}

  virtual void from_uint(std::uintmax_t value, node_data* data,
                         std::size_t index, bool param = false) const {}

  virtual void from_real(double value, node_data* data,
                         std::size_t index, bool param = false) const {}

  virtual void from_string(std::string const& value, node_data* data,
                           std::size_t index, bool param = false) const {}

  template<typename Object>
  void from_object(Object* value, node_data* data,
                   std::size_t index, bool param = false) const
  {
    from_pointer(reinterpret_cast<void*>(value), data, index, param);
  }

  virtual std::vector<bool> as_bool_vector(node_data const* data,
                                           std::size_t index,
                                           bool param = false) const
  {
    return {};
  }

  virtual std::vector<std::uint8_t> as_byte_vector(node_data const* data,
                                                   std::size_t index,
                                                   bool param = false) const
  {
    return {};
  }

  virtual std::vector<std::intmax_t> as_int_vector(node_data const* data,
                                                   std::size_t index,
                                                   bool param = false) const
  {
    return {};
  }

  virtual std::vector<std::uintmax_t> as_uint_vector(node_data const* data,
                                                     std::size_t index,
                                                     bool param = false) const
  {
    return {};
  }

  virtual std::vector<double> as_real_vector(node_data const* data,
                                             std::size_t index,
                                             bool param = false) const
  {
    return {};
  }

  virtual std::vector<std::string> as_string_vector(node_data const* data,
                                                    std::size_t index,
                                                    bool param = false) const
  {
    return {};
  }

  template<typename Object>
  std::vector<Object*> as_object_vector(node_data const* data,
                                        std::size_t index,
                                        bool param = false) const
  {
    auto vector = as_pointer_vector(data, index, param);
    return { reinterpret_cast<Object**>(vector.data()), vector.size() };
  }

  virtual void from_bool_vector(std::vector<bool> value, node_data* data,
                                std::size_t index, bool param = false) const {}

  virtual void from_byte_vector(std::vector<std::uint8_t> value, node_data* data,
                                std::size_t index, bool param = false) const {}

  virtual void from_int_vector(std::vector<std::intmax_t> value, node_data* data,
                               std::size_t index, bool param = false) const {}

  virtual void from_uint_vector(std::vector<std::uintmax_t> value, node_data* data,
                                std::size_t index, bool param = false) const {}

  virtual void from_real_vector(std::vector<double> value, node_data* data,
                                std::size_t index, bool param = false) const {}

  virtual void from_string_vector(std::vector<std::string> const& value, node_data* data,
                                  std::size_t index, bool param = false) const {}

  template<typename Object>
  void from_object_vector(std::vector<Object*> value, node_data* data,
                          std::size_t index, bool param = false) const
  {
    from_pointer_vector({ reinterpret_cast<void**>(value.data()), value.size() },
                        data, index, param);
  }

  operator type*()
  {
    return this;
  }

  operator type const*() const
  {
    return this;
  }

  type& operator*()
  {
    return *this;
  }

  type const& operator*() const
  {
    return *this;
  }

  type* operator->()
  {
    return this;
  }

  type const* operator->() const
  {
    return this;
  }

protected:
  virtual void* as_pointer(node_data const* data,
                           std::size_t index,
                           bool param = false) const
  {
    return nullptr;
  }

  virtual void from_pointer(void* value, node_data* data,
                            std::size_t index, bool param = false) const {}

  virtual std::vector<void*> as_pointer_vector(node_data const* data,
                                              std::size_t index,
                                              bool param = false) const
  {
    return {};
  }

  virtual void from_pointer_vector(std::vector<void*> value, node_data* data,
                                   std::size_t index, bool param = false) const {}
};

} /* namespace nodal */
