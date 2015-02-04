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

#ifndef __NODAL_NODE_DATA_H__
#define __NODAL_NODE_DATA_H__

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
  T& input_block()
  {
    return *reinterpret_cast<T*>(input_block_ptr(sizeof(T)));
  }

  template<typename T>
  T const& input_block() const
  {
    return *reinterpret_cast<T const*>(input_block_ptr(sizeof(T)));
  }

  template<typename T>
  T& params_block()
  {
    return *reinterpret_cast<T*>(params_block_ptr(sizeof(T)));
  }

  template<typename T>
  T const& params_block() const
  {
    return *reinterpret_cast<T const*>(params_block_ptr(sizeof(T)));
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
  virtual void* input_block_ptr(std::size_t size) const;
  virtual void* params_block_ptr(std::size_t size) const;

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

template<typename T, typename... Fields>
struct data_block
{
  using type = T;

  static const std::size_t field_size[sizeof...(Fields)];

  static const std::ptrdiff_t field_offset[sizeof...(Fields)];
};

template<typename T, typename... Fields>
const std::size_t data_block<T, Fields...>::field_size[sizeof...(Fields)] =
  { Fields::size... };

template<typename T, typename... Fields>
const std::ptrdiff_t data_block<T, Fields...>::field_offset[sizeof...(Fields)] =
  { Fields::offset... };

template<typename... Fields>
struct data_block<void, Fields...>
{
  static_assert(sizeof...(Fields) == 0, "Fields specified for void data block");
};

using no_data_block = data_block<void>;

namespace detail
{

  template<typename InputBlock, typename ParamsBlock>
  class node_data_impl : public node_data
  {
  public:
    node_data_impl(typename InputBlock::type const& input_block,
                   typename ParamsBlock::type const& params_block)
      : input_block(input_block), params_block(params_block)
      {}

    node_data* clone() const override
    {
      return new node_data_impl(input_block, params_block);
    }

    typename InputBlock::type input_block;
    typename ParamsBlock::type params_block;

  private:
    void* input_block_ptr(std::size_t size) const override
    {
      if (size != sizeof(typename InputBlock::type))
        throw std::bad_cast();

      return const_cast<typename InputBlock::type*>(&input_block);
    }

    void* params_block_ptr(std::size_t size) const override
    {
      if (size != sizeof(typename ParamsBlock::type))
        throw std::bad_cast();

      return const_cast<typename ParamsBlock::type*>(&params_block);
    }

    void* input_ptr(std::size_t index, std::size_t size) const override
    {
      if (size != InputBlock::field_size[index])
        throw std::bad_cast();

      return const_cast<char*>(
        reinterpret_cast<char const*>(&input_block) +
        InputBlock::field_offset[index]);
    }

    void* param_ptr(std::size_t index, std::size_t size) const override
    {
      if (size != ParamsBlock::field_size[index])
        throw std::bad_cast();

      return const_cast<char*>(
        reinterpret_cast<char const*>(&params_block) +
        ParamsBlock::field_offset[index]);
    }
  };

  template<typename InputBlock>
  class node_data_impl<InputBlock, no_data_block> : public node_data
  {
  public:
    node_data_impl(typename InputBlock::type const& input_block)
      : input_block(input_block)
      {}

    node_data* clone() const override
    {
      return new node_data_impl(input_block);
    }

    typename InputBlock::type input_block;

  private:
    void* input_block_ptr(std::size_t size) const override
    {
      if (size != sizeof(typename InputBlock::type))
        throw std::bad_cast();

      return const_cast<typename InputBlock::type*>(&input_block);
    }

    void* input_ptr(std::size_t index, std::size_t size) const override
    {
      if (size != InputBlock::field_size[index])
        throw std::bad_cast();

      return const_cast<char*>(
        reinterpret_cast<char const*>(&input_block) +
        InputBlock::field_offset[index]);
    }
  };

  template<typename ParamsBlock>
  class node_data_impl<no_data_block, ParamsBlock> : public node_data
  {
  public:
    node_data_impl(typename ParamsBlock::type const& params_block)
      : params_block(params_block)
      {}

    node_data* clone() const override
    {
      return new node_data_impl(params_block);
    }

    typename ParamsBlock::type params_block;

  private:
    void* params_block_ptr(std::size_t size) const override
    {
      if (size != sizeof(typename ParamsBlock::type))
        throw std::bad_cast();

      return const_cast<typename ParamsBlock::type*>(&params_block);
    }

    void* param_ptr(std::size_t index, std::size_t size) const override
    {
      if (size != ParamsBlock::field_size[index])
        throw std::bad_cast();

      return const_cast<char*>(
        reinterpret_cast<char const*>(&params_block) +
        ParamsBlock::field_offset[index]);
    }
  };

  template<>
  class node_data_impl<no_data_block, no_data_block> : public node_data
  {
  public:
    node_data* clone() const override
    {
      return new node_data_impl;
    }
  };

} /* namespace detail */

template<typename InputBlock, typename ParamsBlock>
typename std::enable_if<
  !std::is_same<InputBlock, no_data_block>::value &&
  !std::is_same<ParamsBlock, no_data_block>::value,
  node_data*
>::type
make_node_data(typename InputBlock::type const& input_block,
               typename ParamsBlock::type const& params_block)
{
  return new detail::node_data_impl<InputBlock, ParamsBlock>(
    input_block, params_block);
}

template<typename InputBlock, typename ParamsBlock>
typename std::enable_if<
  !std::is_same<InputBlock, no_data_block>::value &&
  std::is_same<ParamsBlock, no_data_block>::value,
  node_data*
>::type
make_node_data(typename InputBlock::type const& input_block)
{
  return new detail::node_data_impl<InputBlock, ParamsBlock>(input_block);
}

template<typename InputBlock, typename ParamsBlock>
typename std::enable_if<
  std::is_same<InputBlock, no_data_block>::value &&
  !std::is_same<ParamsBlock, no_data_block>::value,
  node_data*
>::type
make_node_data(typename ParamsBlock::type const& params_block)
{
  return new detail::node_data_impl<InputBlock, ParamsBlock>(params_block);
}

template<typename InputBlock, typename ParamsBlock>
typename std::enable_if<
  std::is_same<InputBlock, no_data_block>::value &&
  std::is_same<ParamsBlock, no_data_block>::value,
  node_data*
>::type
make_node_data()
{
  return new detail::node_data_impl<InputBlock, ParamsBlock>;
}

} /* namespace nodal */

#endif /* __NODAL_NODE_DATA_H__ */
