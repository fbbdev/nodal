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

#include "graph_node.h"

#include <cstddef>

namespace nodal
{

class graph_link
{
public:
  struct hash
  {
    std::size_t operator()(graph_link const& link) const;
  };

  graph_link() = default;

  graph_link(graph_node* source_node, std::size_t source_socket,
             graph_node* target_node, std::size_t target_socket)
    : source_node(source_node), source_socket(source_socket),
      target_node(target_node), target_socket(target_socket)
    {}

  graph_node* source_node;
  std::size_t source_socket;

  graph_node* target_node;
  std::size_t target_socket;

  bool operator==(graph_link const& other) const;
  bool operator!=(graph_link const& other) const
  {
    return !(*this == other);
  }
};

} /* namespace nodal */

#include "detail/link_list.h"
