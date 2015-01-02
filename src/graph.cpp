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

#include "graph.h"

#include <algorithm>
#include <stdexcept>
#include <unordered_map>

using namespace nodal;

graph::graph(graph const& other)
{
  std::unordered_map<graph_node*, graph_node*> node_map;

  for (auto node: other._nodes) {
    node_map.emplace(node, add(new graph_node(*node)));
  }

  for (auto link: other._links) {
    this->link(node_map.at(link.source_node), link.source_socket,
               node_map.at(link.target_node), link.target_socket);
  }
}

graph::graph(graph&& other)
  : _nodes(std::move(other._nodes)), _links(std::move(other._links))
  {}

graph::~graph()
{
  for (auto node: _nodes)
    delete node;
}

graph& graph::operator=(graph const& other)
{
  std::unordered_map<graph_node*, graph_node*> node_map;

  clear();

  for (auto node: other._nodes) {
    node_map.emplace(node, add(new graph_node(*node)));
  }

  for (auto link: other._links) {
    this->link(node_map.at(link.source_node), link.source_socket,
               node_map.at(link.target_node), link.target_socket);
  }

  return *this;
}

graph& graph::operator=(graph&& other)
{
  std::swap(_nodes, other._nodes);
  std::swap(_links, other._links);

  return *this;
}

void graph::clear()
{
  for (auto node: _nodes)
    delete node;

  _nodes.clear();
  _links.clear();
}

graph::node_iterator graph::remove(node_iterator iter)
{
  unlink(iter);
  delete *iter;
  return _nodes.erase(iter);
}

graph::node_iterator graph::remove(node_range range)
{
  for (auto it = range.first; it != range.second; ++it) {
    unlink(it);
    delete *it;
  }

  return _nodes.erase(range.first, range.second);
}

void graph::remove(graph_node* node)
{
  auto it = _nodes.find(node);

  if (it != _nodes.end()) {
    unlink(node);
    delete node;
    _nodes.erase(it);
  }
}

graph_link const& graph::link(graph_link const& link)
{
  if (!has(link.source_node) || !has(link.target_node))
    throw std::out_of_range("Node not in graph");

  if (link.source_socket >= link.source_node->node()->output_count() ||
      link.target_socket >= link.target_node->node()->input_count())
    throw std::out_of_range("Socket index out of range");

  return *_links.insert(link).first;
}

graph_link const& graph::link(graph_node* source_node,
                              std::size_t source_socket,
                              graph_node* target_node,
                              std::size_t target_socket)
{
  if (!has(source_node) || !has(target_node))
    throw std::out_of_range("Node not in graph");

  if (source_socket >= source_node->node()->output_count() ||
      target_socket >= target_node->node()->input_count())
    throw std::out_of_range("Socket index out of range");

  return *_links.emplace(source_node, source_socket,
                         target_node, target_socket).first;
}

void graph::unlink(node_range range)
{
  for (auto it = range.first; it != range.second; ++it)
    unlink(it);
}

void graph::unlink_inputs(node_range range)
{
  for (auto it = range.first; it != range.second; ++it)
    unlink_inputs(it);
}

void graph::unlink_outputs(node_range range)
{
  for (auto it = range.first; it != range.second; ++it)
    unlink_inputs(it);
}

void graph::unlink_input(graph_node* node, std::size_t socket)
{
  auto range = _links.get<detail::target_index>().equal_range(node);
  auto it = std::find_if(range.first, range.second,
                         [socket](graph_link const& link)
                         {
                           return link.target_socket == socket;
                         });

  _links.get<detail::target_index>().erase(it);
}

void graph::unlink_output(graph_node* node, std::size_t socket)
{
  auto range = _links.get<detail::source_index>().equal_range(node);
  auto it = std::find_if(range.first, range.second,
                         [socket](graph_link const& link)
                         {
                           return link.target_socket == socket;
                         });

  _links.get<detail::source_index>().erase(it);
}

void boost::remove_edge(nodal::graph_node* u, nodal::graph_node* v,
                        nodal::graph& g)
{
  auto range = g.output_links(u);

  for (auto it = range.first; it != range.second;)
  {
    if (it->target_node == v)
      it = g.unlink(it);
    else
      ++it;
  }
}
