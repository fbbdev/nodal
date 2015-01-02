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

#ifndef __NODAL_GRAPH_H__
#define __NODAL_GRAPH_H__

#include "graph_node.h"
#include "graph_link.h"

#include <set>

namespace nodal
{

class graph
{
public:
  using node_iterator = std::set<graph_node*>::const_iterator;
  using node_range = std::pair<node_iterator, node_iterator>;

  using link_iterator = detail::link_list::const_iterator;
  using link_range = std::pair<link_iterator, link_iterator>;

  using input_link_iterator =
    detail::link_list::index<detail::target_index>::type::const_iterator;
  using input_link_range = std::pair<input_link_iterator, input_link_iterator>;

  using output_link_iterator =
    detail::link_list::index<detail::source_index>::type::const_iterator;
  using output_link_range =
    std::pair<output_link_iterator, output_link_iterator>;

  graph() = default;

  graph(graph const& other);
  graph(graph&& other);

  ~graph();

  graph& operator=(graph const&);
  graph& operator=(graph&&);

  void clear();
  void clear_links()
  {
    _links.clear();
  }

  graph_node* add(graph_node* node)
  {
    if (node)
      _nodes.insert(node);

    return node;
  }

  template<typename... Args>
  graph_node* add(class node const* node)
  {
    return *_nodes.emplace(new graph_node(node)).first;
  }

  node_iterator remove(node_iterator iter);
  node_iterator remove(node_range range);
  void remove(graph_node* node);

  node_iterator find(graph_node* node) const
  {
    return _nodes.find(node);
  }

  node_iterator nodes_begin() const
  {
    return _nodes.cbegin();
  }

  node_iterator nodes_end() const
  {
    return _nodes.cend();
  }

  node_range nodes() const
  {
    return { _nodes.cbegin(), _nodes.cend() };
  }

  std::size_t node_count() const
  {
    return _nodes.size();
  }

  bool has(graph_node* node) const
  {
    return _nodes.count(node);
  }

  graph_link const& link(graph_link const& link);

  graph_link const& link(node_iterator source_iter, std::size_t source_socket,
                         node_iterator target_iter, std::size_t target_socket)
  {
    return link(*source_iter, source_socket, *target_iter, target_socket);
  }

  graph_link const& link(graph_node* source_node, std::size_t source_socket,
                         graph_node* target_node, std::size_t target_socket);

  link_iterator unlink(link_iterator iter)
  {
    return _links.erase(iter);
  }

  link_iterator unlink(link_range range)
  {
    return _links.erase(range.first, range.second);
  }

  input_link_iterator unlink(input_link_iterator iter)
  {
    return _links.get<detail::target_index>().erase(iter);
  }

  input_link_iterator unlink(input_link_range range)
  {
    return _links.get<detail::target_index>().erase(range.first, range.second);
  }

  output_link_iterator unlink(output_link_iterator iter)
  {
    return _links.get<detail::source_index>().erase(iter);
  }

  output_link_iterator unlink(output_link_range range)
  {
    return _links.get<detail::source_index>().erase(range.first, range.second);
  }

  void unlink(graph_link link)
  {
    _links.erase(link);
  }

  void unlink(node_iterator iter)
  {
    unlink(*iter);
  }

  void unlink(node_range range);

  void unlink(graph_node* node)
  {
    unlink_inputs(node);
    unlink_outputs(node);
  }

  void unlink_inputs(node_iterator iter)
  {
    unlink_inputs(*iter);
  }

  void unlink_inputs(node_range range);

  void unlink_inputs(graph_node* node)
  {
    _links.get<detail::target_index>().erase(node);
  }

  void unlink_outputs(node_iterator iter)
  {
    unlink_outputs(*iter);
  }

  void unlink_outputs(node_range range);

  void unlink_outputs(graph_node* node)
  {
    _links.get<detail::source_index>().erase(node);
  }

  void unlink_input(node_iterator iter, std::size_t socket)
  {
    unlink_input(*iter, socket);
  }

  void unlink_input(graph_node* node, std::size_t socket);

  void unlink_output(node_iterator iter, std::size_t socket)
  {
    unlink_output(*iter, socket);
  }

  void unlink_output(graph_node* node, std::size_t socket);

  link_iterator find(graph_link const& link) const
  {
    return _links.find(link);
  }

  link_iterator links_begin() const
  {
    return _links.cbegin();
  }

  link_iterator links_end() const
  {
    return _links.cend();
  }

  link_range links() const
  {
    return { _links.cbegin(), _links.cend() };
  }

  std::size_t link_count() const
  {
    return _links.size();
  }

  bool has(graph_link const& link)
  {
    return _links.count(link);
  }

  input_link_range input_links(node_iterator iter) const
  {
    return input_links(*iter);
  }

  input_link_range input_links(graph_node* node) const
  {
    return _links.get<detail::target_index>().equal_range(node);
  }

  std::size_t input_degree(node_iterator iter) const
  {
    return input_degree(*iter);
  }

  std::size_t input_degree(graph_node* node) const
  {
    return _links.get<detail::target_index>().count(node);
  }

  output_link_range output_links(node_iterator iter) const
  {
    return output_links(*iter);
  }

  output_link_range output_links(graph_node* node) const
  {
    return _links.get<detail::source_index>().equal_range(node);
  }

  std::size_t output_degree(node_iterator iter) const
  {
    return output_degree(*iter);
  }

  std::size_t output_degree(graph_node* node) const
  {
    return _links.get<detail::source_index>().count(node);
  }

  std::size_t degree(node_iterator iter) const
  {
    return degree(*iter);
  }

  std::size_t degree(graph_node* node) const
  {
    return input_degree(node) + output_degree(node);
  }

private:
  std::set<graph_node*> _nodes;
  detail::link_list _links;
};

} /* namespace nodal */

#include "detail/graph_access.h"
#include "detail/graph_properties.h"

#endif /* __NODAL_GRAPH_H__ */
