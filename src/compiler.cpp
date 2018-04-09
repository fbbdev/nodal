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

#include "compiler.hpp"

using namespace nodal;

void context::set(std::type_index const& pass, any&& data) {
    pass_data[pass] = std::move(data);
}

any context::get(std::type_index const& pass) const {
    if (pass_data.count(pass))
        return pass_data.at(pass);
    else
        return {};
}

compiler::~compiler() {
    for (auto pass : *this)
        if (pass)
            delete pass;
}

any compiler::run(graph& graph, context& ctx) const {
    pass* last_pass;

    for (auto const& pass : *this) {
        auto result = pass->run(graph, ctx);

        if (!result.empty())
            ctx.set(typeid(*pass), std::move(result));

        last_pass = pass;
    }

    return ctx.get(typeid(*last_pass));
}
