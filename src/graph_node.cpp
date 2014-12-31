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

#include "graph_node.h"

using namespace nodal;

graph_node::graph_node(class node const* node)
  : _node(node), inputs(nullptr), properties(nullptr)
{
  if (node) {
    inputs = node->input_data();
    properties = node->property_data();
  }
}

graph_node::graph_node(graph_node const& other)
  : _node(other._node), inputs(nullptr), properties(nullptr),
    attributes(other.attributes)
{
  if (other.inputs)
    inputs = other.inputs->clone();
  if (other.properties)
    properties = other.properties->clone();
}

graph_node::graph_node(graph_node&& other)
  : _node(other._node),
    inputs(other.inputs),
    properties(other.properties),
    attributes(std::move(other.attributes))
{
  other.inputs = other.properties = nullptr;
}

graph_node::~graph_node()
{
  if (inputs)
    delete inputs;
  if (properties)
    delete properties;
}

graph_node& graph_node::operator=(graph_node const& other)
{
  _node = other._node;
  inputs = other.inputs->clone();
  properties = other.properties->clone();
  attributes = other.attributes;

  return *this;
}

graph_node& graph_node::operator=(graph_node&& other)
{
  std::swap(_node, other._node);
  std::swap(inputs, other.inputs);
  std::swap(properties, other.properties);
  std::swap(attributes, other.attributes);

  return *this;
}
