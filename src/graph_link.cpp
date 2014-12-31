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

#include "graph_link.h"

#include <boost/functional/hash.hpp>

using namespace nodal;

namespace boost
{
  namespace multi_index
  {
    template class multi_index_container<
      nodal::graph_link,
      indexed_by<
        hashed_unique<identity<nodal::graph_link>, nodal::graph_link::hash>,
        ordered_non_unique<
          tag<nodal::detail::source_index>,
          member<nodal::graph_link, nodal::graph_node*,
                 &nodal::graph_link::source_node>
        >,
        ordered_non_unique<
          tag<nodal::detail::target_index>,
          member<nodal::graph_link, nodal::graph_node*,
                 &nodal::graph_link::target_node>
        >
      >
    >;
  }
}

std::size_t graph_link::hash::operator()(graph_link const& link) const
{
  std::size_t hash = 0;

  boost::hash_combine(hash, link.source_node);
  boost::hash_combine(hash, link.source_socket);
  boost::hash_combine(hash, link.target_node);
  boost::hash_combine(hash, link.target_socket);

  return hash;
}

bool graph_link::operator==(graph_link const& other) const
{
  return source_node == other.source_node &&
         target_node == other.target_node &&
         source_socket == other.source_socket &&
         target_socket == other.target_socket;
}
