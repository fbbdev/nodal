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
#include <type_traits>
#include <typeinfo>

namespace nodal
{

class node_data {
public:
    virtual ~node_data() {}

    virtual node_data* clone() const = 0;

    template <typename T>
    T& data() {
        return *reinterpret_cast<T*>(data_ptr(sizeof(T)));
    }

    template <typename T>
    T const& data() const {
        return *reinterpret_cast<T const*>(data_ptr(sizeof(T)));
    }

    template <typename T>
    T& input(std::size_t index) {
        return *reinterpret_cast<T*>(input_ptr(index, sizeof(T)));
    }

    template <typename T>
    T const& input(std::size_t index) const {
        return *reinterpret_cast<T const*>(input_ptr(index, sizeof(T)));
    }

    template <typename T>
    T& param(std::size_t index) {
        return *reinterpret_cast<T*>(param_ptr(index, sizeof(T)));
    }

    template <typename T>
    T const& param(std::size_t index) const {
        return *reinterpret_cast<T const*>(param_ptr(index, sizeof(T)));
    }

private:
    virtual void* data_ptr(std::size_t size) const;

    virtual void* input_ptr(std::size_t index, std::size_t size) const;
    virtual void* param_ptr(std::size_t index, std::size_t size) const;
};

template <typename S, typename T, T S::*Member>
struct data_field {
    static constexpr std::size_t size = sizeof(T);
    static constexpr void* ptr(void* data) {
        return &(reinterpret_cast<S*>(data)->*Member);
    }
};

template <typename... Fields>
struct data_block {
    using ptr_fn_type = void* (*) (void*);

    static constexpr std::size_t field_count = sizeof...(Fields);
    static const std::size_t field_size[sizeof...(Fields)];
    static const ptr_fn_type field_ptr[sizeof...(Fields)];
};

template <typename... Fields>
const std::size_t data_block<Fields...>::field_size[sizeof...(Fields)] =
    { Fields::size... };

template <typename... Fields>
const typename data_block<Fields...>::ptr_fn_type
data_block<Fields...>::field_ptr[sizeof...(Fields)] =
    { &Fields::ptr... };

template <>
struct data_block<> {};

using no_data_block = data_block<>;

namespace detail
{

    template <typename T, typename InputBlock, typename ParamBlock>
    class struct_node_data_impl : public node_data {
    public:
        template <typename... Args>
        struct_node_data_impl(Args&&... args)
            : data(std::forward<Args>(args)...)
            {}

        node_data* clone() const override {
            return new struct_node_data_impl(data);
        }

        T data;

    private:
        void* data_ptr(std::size_t size) const override {
            if (size != sizeof(T))
                throw std::bad_cast();

            return const_cast<T*>(&data);
        }

        void* input_ptr(std::size_t index, std::size_t size) const override {
            if (index >= InputBlock::field_count)
                throw std::out_of_range("struct_node_data_impl::input_ptr");

            if (size != InputBlock::field_size[index])
                throw std::bad_cast();

            return InputBlock::field_ptr[index](const_cast<T*>(&data));
        }

        void* param_ptr(std::size_t index, std::size_t size) const override {
            if (index >= ParamBlock::field_count)
                throw std::out_of_range("struct_node_data_impl::param_ptr");

            if (size != ParamBlock::field_size[index])
                throw std::bad_cast();

            return ParamBlock::field_ptr[index](const_cast<T*>(&data));
        }
    };

    template <typename T, typename InputBlock>
    class struct_node_data_impl<T, InputBlock, no_data_block>
        : public node_data {
    public:
        template <typename... Args>
        struct_node_data_impl(Args&&... args)
            : data(std::forward<Args>(args)...) {}

        node_data* clone() const override {
            return new struct_node_data_impl(data);
        }

        T data;

    private:
        void* data_ptr(std::size_t size) const override {
            if (size != sizeof(T))
                throw std::bad_cast();

            return const_cast<T*>(&data);
        }

        void* input_ptr(std::size_t index, std::size_t size) const override {
            if (index >= InputBlock::field_count)
                throw std::out_of_range("struct_node_data_impl::input_ptr");

            if (size != InputBlock::field_size[index])
                throw std::bad_cast();

            return InputBlock::field_ptr[index](const_cast<T*>(&data));
        }
    };

    template <typename T, typename ParamBlock>
    class struct_node_data_impl<T, no_data_block, ParamBlock>
        : public node_data {
    public:
        template <typename... Args>
        struct_node_data_impl(Args&&... args)
            : data(std::forward<Args>(args)...)
            {}

        node_data* clone() const override {
            return new struct_node_data_impl(data);
        }

        T data;

    private:
        void* data_ptr(std::size_t size) const override {
            if (size != sizeof(T))
                throw std::bad_cast();

            return const_cast<T*>(&data);
        }

        void* param_ptr(std::size_t index, std::size_t size) const override {
            if (index >= ParamBlock::field_count)
                throw std::out_of_range("struct_node_data_impl::param_ptr");

            if (size != ParamBlock::field_size[index])
                throw std::bad_cast();

            return ParamBlock::field_ptr[index](const_cast<T*>(&data));
        }
    };

    template <typename T>
    class struct_node_data_impl<T, no_data_block, no_data_block>
        : public node_data {
    public:
        template <typename... Args>
        struct_node_data_impl(Args&&... args)
            : data(std::forward<Args>(args)...)
            {}

        node_data* clone() const override {
            return new struct_node_data_impl(data);
        }

        T data;

    private:
        void* data_ptr(std::size_t size) const override {
            if (size != sizeof(T))
                throw std::bad_cast();

            return const_cast<T*>(&data);
        }
    };

} /* namespace detail */

template <typename T, typename InputBlock,
          typename ParamBlock, typename... Args>
node_data* struct_node_data(Args&&... args) {
    return new detail::struct_node_data_impl<T, InputBlock, ParamBlock>(
        std::forward<Args>(args)...);
}

} /* namespace nodal */
