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

#include "nodes/all.h"

#include <nodal/graph_node.h>

struct node_factory {
    ~node_factory() {
        delete input_n;
        delete output_n;
        delete math_n;
        delete random_n;
    }

    nodal::graph_node* input(std::string const& name, std::size_t index) const;

    nodal::graph_node* output(std::string const& name, std::size_t index) const;

    nodal::graph_node* math(std::string const& name,
                            math_node::function fn) const;

    nodal::graph_node* random(std::string const& name) const;

    nodal::node* input_n = new input_node;
    nodal::node* output_n = new output_node;
    nodal::node* math_n = new math_node;
    nodal::node* random_n = new random_node;
};
