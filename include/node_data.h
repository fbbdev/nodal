/**
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

#include <cstddef>
#include <limits>

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
    return *reinterpret_cast<T*>(data_ptr());
  }

  template<typename T>
  T const& data() const
  {
    return *reinterpret_cast<T const*>(data_ptr());
  }

  template<typename T>
  T& field(std::size_t index)
  {
    return *reinterpret_cast<T*>(field_ptr(index));
  }

  template<typename T>
  T const& field(std::size_t index) const
  {
    return *reinterpret_cast<T const*>(field_ptr(index));
  }

protected:
  virtual void* data_ptr() const = 0;
  virtual void* field_ptr(std::size_t index) const = 0;
};

namespace detail
{

  template<typename C, std::size_t Index,
           std::ptrdiff_t Field = std::numeric_limits<std::ptrdiff_t>::max(),
           std::ptrdiff_t... Fields>
  struct field_access_impl
  {
    static void* get(C* container, std::size_t index)
    {
      if (index == Index)
        return ((char*) container) + Field;
      else
        return field_access_impl<C, Index+1, Fields...>::get(container, index);
    }
  };

  template<typename C, std::size_t Index>
  struct field_access_impl<C, Index, std::numeric_limits<std::ptrdiff_t>::max()>
  {
    static void* get(C* container, std::size_t index)
    {
      return nullptr;
    }
  };

  template<typename C, std::ptrdiff_t... Fields>
  using field_access = field_access_impl<C, 0, Fields...>;

  template<typename C, std::ptrdiff_t... Fields>
  class node_data_impl : public node_data
  {
  public:
    node_data_impl(C const& data) : container(data) {}

    node_data* clone() const override
    {
      return new node_data_impl(container);
    }

    C container;

  private:
    void* data_ptr() const override
    {
      return const_cast<C*>(&container);
    }

    void* field_ptr(std::size_t index) const override
    {
      return field_access<C, Fields...>::get(const_cast<C*>(&container), index);
    }
  };

} /* namespace detail */

template<typename C, std::ptrdiff_t... Fields>
node_data* make_node_data(C const& data)
{
  return new detail::node_data_impl<C, Fields...>(data);
}

} /* namespace nodal */

#endif /* __NODAL_NODE_DATA_H__ */
