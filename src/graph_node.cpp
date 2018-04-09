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

#include "graph_node.hpp"

using namespace nodal;

graph_node::graph_node(class node const* node)
    : node_(node), data_(nullptr)
{
    if (node)
        data_ = node->data();
}

graph_node::graph_node(graph_node const& other)
    : node_(other.node_), data_(nullptr), attributes(other.attributes)
{
    if (other.data_)
        data_ = other.data_->clone();
}

graph_node::graph_node(graph_node&& other)
    : node_(other.node_), data_(other.data_),
      attributes(std::move(other.attributes))
{
    other.data_ = nullptr;
}

graph_node::~graph_node() {
    if (data_)
        delete data_;
}

graph_node& graph_node::operator=(graph_node const& other) {
    node_ = other.node_;
    data_ = other.data_->clone();
    attributes = other.attributes;

    return *this;
}

graph_node& graph_node::operator=(graph_node&& other) {
    std::swap(node_, other.node_);
    std::swap(data_, other.data_);
    std::swap(attributes, other.attributes);

    return *this;
}
