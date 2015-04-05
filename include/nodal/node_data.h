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

#include <stdexcept>
#include <string>
#include <typeinfo>
#include <type_traits>

namespace nodal
{

class node_data
{
public:
  virtual ~node_data() {}

  virtual node_data* clone() const = 0;

  template<typename T>
  T& data()
  {
    return *reinterpret_cast<T*>(data_ptr(sizeof(T)));
  }

  template<typename T>
  T const& data() const
  {
    return *reinterpret_cast<T const*>(data_ptr(sizeof(T)));
  }

  template<typename T>
  T& input(std::size_t index)
  {
    return *reinterpret_cast<T*>(input_ptr(index, sizeof(T)));
  }

  template<typename T>
  T const& input(std::size_t index) const
  {
    return *reinterpret_cast<T const*>(input_ptr(index, sizeof(T)));
  }

  template<typename T>
  T& input(std::string const& key)
  {
    return *reinterpret_cast<T*>(input_ptr(key, sizeof(T)));
  }

  template<typename T>
  T const& input(std::string const& key) const
  {
    return *reinterpret_cast<T const*>(input_ptr(key, sizeof(T)));
  }

  template<typename T>
  T& param(std::size_t index)
  {
    return *reinterpret_cast<T*>(param_ptr(index, sizeof(T)));
  }

  template<typename T>
  T const& param(std::size_t index) const
  {
    return *reinterpret_cast<T const*>(param_ptr(index, sizeof(T)));
  }

  template<typename T>
  T& param(std::string const& key)
  {
    return *reinterpret_cast<T*>(param_ptr(key, sizeof(T)));
  }

  template<typename T>
  T const& param(std::string const& key) const
  {
    return *reinterpret_cast<T const*>(param_ptr(key, sizeof(T)));
  }

private:
  virtual void* data_ptr(std::size_t size) const;

  virtual void* input_ptr(std::size_t index, std::size_t size) const;
  virtual void* input_ptr(std::string const& key, std::size_t size) const;

  virtual void* param_ptr(std::size_t index, std::size_t size) const;
  virtual void* param_ptr(std::string const& key, std::size_t size) const;
};

template<typename T, std::ptrdiff_t Offset>
struct data_field
{
  static constexpr std::size_t size = sizeof(T);
  static constexpr std::ptrdiff_t offset = Offset;
};

template<typename... Fields>
struct data_block
{
  static constexpr std::size_t field_count = sizeof...(Fields);
  static const std::size_t field_size[sizeof...(Fields)];
  static const std::ptrdiff_t field_offset[sizeof...(Fields)];
};

template<typename... Fields>
const std::size_t data_block<Fields...>::field_size[sizeof...(Fields)] =
  { Fields::size... };

template<typename... Fields>
const std::ptrdiff_t data_block<Fields...>::field_offset[sizeof...(Fields)] =
  { Fields::offset... };

template<> struct data_block<> {};
using no_data_block = data_block<>;

namespace detail
{

  template<typename T, typename InputBlock, typename ParamBlock>
  class node_data_impl : public node_data
  {
  public:
    template<typename... Args>
    node_data_impl(Args&&... args) : data(std::forward<Args>(args)...) {}

    node_data* clone() const override
    {
      return new node_data_impl(data);
    }

    T data;

  private:
    void* data_ptr(std::size_t size) const override
    {
      if (size != sizeof(T))
        throw std::bad_cast();

      return const_cast<T*>(&data);
    }

    void* input_ptr(std::size_t index, std::size_t size) const override
    {
      if (index >= InputBlock::field_count)
        throw std::out_of_range("node_data_impl::input_ptr");

      if (size != InputBlock::field_size[index])
        throw std::bad_cast();

      return const_cast<char*>(
        reinterpret_cast<char const*>(&data) +
        InputBlock::field_offset[index]);
    }

    void* param_ptr(std::size_t index, std::size_t size) const override
    {
      if (index >= ParamBlock::field_count)
        throw std::out_of_range("node_data_impl::param_ptr");

      if (size != ParamBlock::field_size[index])
        throw std::bad_cast();

      return const_cast<char*>(
        reinterpret_cast<char const*>(&data) +
        ParamBlock::field_offset[index]);
    }
  };

  template<typename T, typename InputBlock>
  class node_data_impl<T, InputBlock, no_data_block> : public node_data
  {
  public:
    template<typename... Args>
    node_data_impl(Args&&... args) : data(std::forward<Args>(args)...) {}

    node_data* clone() const override
    {
      return new node_data_impl(data);
    }

    T data;

  private:
    void* data_ptr(std::size_t size) const override
    {
      if (size != sizeof(T))
        throw std::bad_cast();

      return const_cast<T*>(&data);
    }

    void* input_ptr(std::size_t index, std::size_t size) const override
    {
      if (index >= InputBlock::field_count)
        throw std::out_of_range("node_data_impl::input_ptr");

      if (size != InputBlock::field_size[index])
        throw std::bad_cast();

      return const_cast<char*>(
        reinterpret_cast<char const*>(&data) +
        InputBlock::field_offset[index]);
    }
  };

  template<typename T, typename ParamBlock>
  class node_data_impl<T, no_data_block, ParamBlock> : public node_data
  {
  public:
    template<typename... Args>
    node_data_impl(Args&&... args) : data(std::forward<Args>(args)...) {}

    node_data* clone() const override
    {
      return new node_data_impl(data);
    }

    T data;

  private:
    void* data_ptr(std::size_t size) const override
    {
      if (size != sizeof(T))
        throw std::bad_cast();

      return const_cast<T*>(&data);
    }

    void* param_ptr(std::size_t index, std::size_t size) const override
    {
      if (index >= ParamBlock::field_count)
        throw std::out_of_range("node_data_impl::param_ptr");

      if (size != ParamBlock::field_size[index])
        throw std::bad_cast();

      return const_cast<char*>(
        reinterpret_cast<char const*>(&data) +
        ParamBlock::field_offset[index]);
    }
  };

  template<typename T>
  class node_data_impl<T, no_data_block, no_data_block> : public node_data
  {
  public:
    template<typename... Args>
    node_data_impl(Args&&... args) : data(std::forward<Args>(args)...) {}

    node_data* clone() const override
    {
      return new node_data_impl(data);
    }

    T data;

  private:
    void* data_ptr(std::size_t size) const override
    {
      if (size != sizeof(T))
        throw std::bad_cast();

      return const_cast<T*>(&data);
    }
  };

} /* namespace detail */

template<typename T,
         typename InputBlock,
         typename ParamBlock,
         typename... Args>
node_data* make_node_data(Args&&... args)
{
  return new detail::node_data_impl<T, InputBlock, ParamBlock>(
    std::forward<Args>(args)...);
}

} /* namespace nodal */
