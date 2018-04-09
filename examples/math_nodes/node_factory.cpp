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

#include "node_factory.h"

nodal::graph_node* node_factory::input(std::string const& name,
                                       std::size_t index) const {
    auto n = new nodal::graph_node(input_n);
    n->attribute("name") = name;
    n->data()->param<std::size_t>(0) = index;

    return n;
}

nodal::graph_node* node_factory::output(std::string const& name,
                                        std::size_t index) const {
    auto n = new nodal::graph_node(output_n);
    n->attribute("name") = name;
    n->data()->param<std::size_t>(0) = index;

    return n;
}

nodal::graph_node* node_factory::math(std::string const& name,
                                      math_node::function fn) const {
    auto n = new nodal::graph_node(math_n);
    n->attribute("name") = name;
    n->data()->param<math_node::function>(0) = fn;

    return n;
}

nodal::graph_node* node_factory::random(std::string const& name) const {
    auto n = new nodal::graph_node(random_n);
    n->attribute("name") = name;

    return n;
}
