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

#include "any.h"
#include "graph.h"

#include <forward_list>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace nodal
{

class context {
public:
    template <typename Pass>
    typename Pass::result_type const& pass() const {
        return pass_data.at(typeid(Pass));
    }

private:
    friend class compiler;

    void set(std::type_index const& pass, any&& data);
    any get(std::type_index const& pass) const;

    std::unordered_map<std::type_index, any> pass_data;
};

class pass {
public:
    virtual ~pass() {}
    virtual any run(graph& graph, context& ctx) const = 0;
};

class compiler : public std::forward_list<pass*>, public pass {
    using base = std::forward_list<pass*>;

public:
    using base::forward_list;

    ~compiler();

    any run(graph& graph, context& ctx) const override;

    any compile(graph graph, context& ctx) const {
        return run(graph, ctx);
    }

    any compile(graph graph) const {
        context ctx;
        return compile(std::move(graph), ctx);
    }
};

} /* namespace nodal */
